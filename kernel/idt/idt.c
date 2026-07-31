#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(unsigned int);

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

    idt_load((unsigned int)&idtp);
}