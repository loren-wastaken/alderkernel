#include "headers/print.h"
#include "headers/colors.h"
#include "headers/screen.h"
#include "idt/idt.h"
#include "shell/headers/shell.h"
#include "drivers/ps2.h"
#include "interrupts/pic.h"
#include "initramfs.h"
#include "mm/pmm.h"
#include "mm/multiboot.h"
#include "mm/heap.h"
#include "mm/paging.h"

void clear_screen(unsigned char color) {
    unsigned short* buffer = VIDEO_MEMORY;
    unsigned short blank = 0x20 | (color << 8);
    unsigned int size = SCREEN_WIDTH * SCREEN_HEIGHT;

    for(unsigned int i = 0; i < size; i++) {
        buffer[i] = blank;
    }

    clear_screen_reset_cursor();
}

// early hardware bring-up, then hand off to the initramfs stage
void kernel_start(unsigned int multiboot_magic, unsigned int multiboot_info_addr)
{
    asm volatile("cli");

    clear_screen(BLACK);

    pic_remap();
    idt_init();
    ps2_init();

    asm volatile("sti");

    if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        print_text("WARNING: invalid multiboot magic - memory map may be missing.\n");
        pmm_init(0); // pmm_init handles 0 by reserving everything but the kernel itself
    } else {
        pmm_init(multiboot_info_addr);
    }

    print_text("pmm: ");
    print_uint(pmm_get_free_page_count());
    print_text(" / ");
    print_uint(pmm_get_total_page_count());
    print_text(" pages free\n");

    paging_init();
    print_text("paging: enabled (identity-mapped first 128MB)\n");

    heap_init();
    print_text("heap: ");
    print_uint(heap_get_free_bytes());
    print_text(" bytes free\n");

    // initramfs_init() finishes setup (mounts the in-RAM VFS) and
    // then calls kernel_main() below - never returns
    initramfs_init();

    while(1)
    {
        asm volatile("hlt");
    }
}

// real entry point into "the kernel proper" - reached once initramfs
// has finished setting up. never returns.
void kernel_main(void)
{
    print_text("Kernel loaded!");
    shell_start();
}