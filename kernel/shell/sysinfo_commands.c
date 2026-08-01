#include "headers/commands.h"
#include "headers/util.h"
#include "../headers/print.h"
#include "headers/commands.h"
#include "headers/util.h"
#include "../headers/print.h"
#include "../syscalls/syscall.h"
#include "../mm/pmm.h"
#include "../mm/heap.h"
#include "../drivers/ata.h"
#include "../fs/mbr.h"

// command: uname
// options: -a (all), -s (kernel name, default), -r (release), -m (machine)
void command_uname(char* option)
{
    if (option == (char*)0 || strcmp(option, "-s") == 0) {
        print_text("AlderKernel\n");
        return;
    }

    if (strcmp(option, "-r") == 0) {
        print_text("0.1\n");
        return;
    }

    if (strcmp(option, "-m") == 0) {
        print_text("i386\n");
        return;
    }

    if (strcmp(option, "-a") == 0) {
        print_text("AlderKernel 0.1 i386 zSlash\n");
        return;
    }

    print_text("uname: unrecognized option: ");
    print_text(option);
    print_text("\n");
}



// command: req-syscallop <syscall number>
void command_req_syscallop(char* arg)
{
    if (arg == (char*)0) {
        print_text("Usage: req-syscallop <syscall number>\n");
        return;
    }

    unsigned int syscall_num = str_to_uint(arg);

    print_text("Syscall requested: #");
    print_uint(syscall_num);
    print_text("\n");

    syscall_dispatch(syscall_num);
}



// command: memtest
// NAIVE fixed-address memory test - there is no multiboot memory map
// check yet, so this blindly assumes the region below is free, usable
// RAM. That's true on typical QEMU defaults but is NOT a safe
// assumption on real hardware; replace with a real memory-map probe
// before trusting this outside a VM.
#define MEMTEST_BASE  ((volatile unsigned int*)0x400000) // 4MB mark
#define MEMTEST_WORDS 4096u                               // 16KB tested

void command_memtest(void)
{
    unsigned int pattern1 = 0xAAAAAAAA;
    unsigned int pattern2 = 0x55555555;
    unsigned int failures = 0;

    print_text("Running memory test at ");
    print_hex((unsigned int)MEMTEST_BASE);
    print_text(" (");
    print_uint(MEMTEST_WORDS * 4);
    print_text(" bytes)...\n");

    for (unsigned int i = 0; i < MEMTEST_WORDS; i++) {
        MEMTEST_BASE[i] = pattern1;
    }
    for (unsigned int i = 0; i < MEMTEST_WORDS; i++) {
        if (MEMTEST_BASE[i] != pattern1) {
            failures++;
        }
    }

    for (unsigned int i = 0; i < MEMTEST_WORDS; i++) {
        MEMTEST_BASE[i] = pattern2;
    }
    for (unsigned int i = 0; i < MEMTEST_WORDS; i++) {
        if (MEMTEST_BASE[i] != pattern2) {
            failures++;
        }
    }

    if (failures == 0) {
        print_text("Memory test PASSED.\n");
    } else {
        print_text("Memory test FAILED - ");
        print_uint(failures);
        print_text(" word mismatches.\n");
    }
}

// command: meminfo
void command_meminfo(void)
{
    print_text("Physical memory: ");
    print_uint(pmm_get_free_page_count());
    print_text(" / ");
    print_uint(pmm_get_total_page_count());
    print_text(" pages free (");
    print_uint(pmm_get_free_page_count() * 4);
    print_text(" KB)\n");

    print_text("Heap: ");
    print_uint(heap_get_free_bytes());
    print_text(" bytes free, ");
    print_uint(heap_get_used_bytes());
    print_text(" bytes used\n");
}
// command: diskinfo
void command_diskinfo(void)
{
    print_text("Probing primary ATA bus (master)...\n");

    if (!ata_identify()) {
        print_text("No drive detected.\n");
        return;
    }

    print_text("Drive detected. Reading sector 0 (MBR)...\n");

    unsigned char sector[512];
    if (!ata_read_sector(0, sector)) {
        print_text("Read failed.\n");
        return;
    }

    print_text("First 16 bytes: ");
    for (int i = 0; i < 16; i++) {
        print_hex(sector[i]);
        print_text(" ");
    }
    print_text("\n");

    print_text("Boot signature (should be 0xAA55 at offset 510): ");
    print_hex(sector[510] | (sector[511] << 8));
    print_text("\n");
}
// command: partinfo
void command_partinfo(void)
{
    mbr_partition_t parts[4];
    int used = mbr_read_partitions(parts);

    if (used == 0) {
        print_text("No valid MBR found (disk is unpartitioned).\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (parts[i].type == 0) {
            continue;
        }

        print_text("Partition ");
        print_uint(i);
        print_text(": type=");
        print_hex(parts[i].type);
        print_text(" bootable=");
        print_uint(parts[i].bootable);
        print_text(" start_lba=");
        print_uint(parts[i].lba_start);
        print_text(" sectors=");
        print_uint(parts[i].sector_count);
        print_text("\n");
    }
}