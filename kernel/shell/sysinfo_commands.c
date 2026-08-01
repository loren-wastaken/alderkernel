#include "headers/commands.h"
#include "headers/util.h"
#include "../headers/print.h"
#include "headers/commands.h"
#include "headers/util.h"
#include "../headers/print.h"
#include "../syscalls/syscall.h"

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
