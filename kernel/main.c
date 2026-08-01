#include "headers/print.h"
#include "headers/colors.h"
#include "headers/screen.h"
#include "idt/idt.h"
#include "shell/headers/shell.h"
#include "drivers/ps2.h"
#include "interrupts/pic.h"
#include "initramfs.h"

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
void kernel_start()
{
    asm volatile("cli");

    clear_screen(BLACK);

    pic_remap();
    idt_init();
    ps2_init();

    asm volatile("sti");

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
    shell_start();
}
