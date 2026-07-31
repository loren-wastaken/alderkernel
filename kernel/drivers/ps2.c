#include "../headers/io.h"
#include "../headers/print.h"

void keyboard_handler()
{
    unsigned char scancode;

    scancode = inb(0x60);

    print_text("KEYS\n");

    outb(0x20, 0x20);
}