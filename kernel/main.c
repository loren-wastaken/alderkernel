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

void kernel_start() {
    clear_screen(BLACK); 
    
    // init interrupt descriptor table
    idt_init();

    print_text("ALDER KERNEL LOADED SUCCESSFULLY\n");
    print_text("Welcome to Alder Kernel, there's no shell but\nit will be built later.\n");
    print_text("Thank you for atleast turning on this Kernel!\nFor now, it's uncontrollable.");
    
    while(1) {
        __asm__ __volatile__("hlt");
    }
}
