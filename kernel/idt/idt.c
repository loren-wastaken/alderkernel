#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(unsigned int);
extern void keyboard_stub();

void idt_set_gate(int n, unsigned int handler, unsigned short selector, unsigned char flags) {
    idt[n].base_low  = handler & 0xFFFF;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
    idt[n].selector  = selector;
    idt[n].zero      = 0;
    idt[n].flags     = flags;
}

void idt_init() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (unsigned int)&idt;
    
    for (int i = 0; i < 256; i++)
{
    idt[i].base_low = 0;
    idt[i].base_high = 0;
    idt[i].selector = 0;
    idt[i].zero = 0;
    idt[i].flags = 0;
}

    idt_set_gate(
    33,
    (unsigned int)keyboard_stub,
    0x08,
    0x8E
);
    idt_load((unsigned int)&idtp);
}