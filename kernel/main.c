#include <stdint.h>
#include "headers/print.h"
#include "headers/colors.h"
#include "headers/screen.h"
#include "idt/idt.h"

volatile int is_cleared = 0;

void clear_screen(unsigned char color) {
    unsigned short* buffer = VIDEO_MEMORY;
    unsigned short blank = 0x20 | (color << 8);
    unsigned int size = SCREEN_WIDTH * SCREEN_HEIGHT;
    
    if (is_cleared == 1) {
        return;
    }

    for(unsigned int i = 0; i < size; i++) {
        buffer[i] = blank;
    }
    is_cleared = 1;
}

/* Multiboot structures (v1) */
typedef struct multiboot_mod {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t cmdline;
    uint32_t reserved;
} multiboot_mod_t;

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    /* ... rest not used here ... */
} multiboot_info_t;

extern int elf_load_from_module(uint32_t mod_start);

/* kernel_start now receives a single argument: multiboot info pointer */
void kernel_start(uint32_t mbd_ptr) {
    multiboot_info_t *mb = (multiboot_info_t *)mbd_ptr;

    clear_screen(BLACK); 
    
    // init interrupt descriptor table
    idt_init();

    print_text("ALDER KERNEL LOADED SUCCESSFULLY\n");
    print_text("Welcome to Alder Kernel, there's no shell but\nit will be built later.\n");
    print_text("Thank you for atleast turning on this Kernel!\nFor now, it's uncontrollable.\n");
    
    // If GRUB provided modules, try to load the first one as an ELF
    if (mb && (mb->flags & 0x8) && mb->mods_count > 0) {
        multiboot_mod_t *mods = (multiboot_mod_t*)mb->mods_addr;
        uint32_t mod_start = mods[0].mod_start;
        print_text("Module detected, attempting to load...\n");
        if (elf_load_from_module(mod_start) == 0) {
            print_text("Module returned to kernel (entry returned)\n");
        } else {
            print_text("Module load failed\n");
        }
    } else {
        print_text("no modules provided\n");
    }

    // fallback loop
    while(1) {
        __asm__ __volatile__("hlt");
    }
}
