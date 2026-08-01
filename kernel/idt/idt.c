#include "idt.h"
#include "../kernel/headers/print.h"

void idt_debug()
{
    print_text("IDT loaded\n");

    print_text("isr13 address:\n");
}


struct idt_entry idt[256];
struct idt_ptr idtp;


extern void idt_load(unsigned int);
extern void keyboard_stub();
extern void isr13();
extern void isr3();


void idt_set_gate(
int n,
unsigned int handler,
unsigned short selector,
unsigned char flags
)
{
    idt[n].base_low = handler & 0xFFFF;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].flags = flags;
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}


void idt_init()
{

    for(int i=0;i<256;i++)
    {
        idt[i].base_low = 0;
        idt[i].base_high = 0;
        idt[i].selector = 0;
        idt[i].flags = 0;
        idt[i].zero = 0;
    }


    idt_set_gate(
        33,
        (unsigned int)keyboard_stub,
        0x08,
        0x8E
    );

    idt_set_gate(
        13,
        (unsigned int)isr13,
        0x08,
        0x8E
    );
    print_text("isr13 installed\n");

    idt_set_gate(
        3,
        (unsigned int)isr3,
        0x08,
        0x8E
    );

    idtp.limit = sizeof(idt)-1;
    idtp.base = (unsigned int)&idt;

    // IDT must be loaded via lidt BEFORE any 'int' instruction is executed,
    // otherwise the CPU faults with a stale/invalid IDTR -> triple fault.
    idt_load((unsigned int)&idtp);
}