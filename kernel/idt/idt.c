#include "idt.h"
#include "../headers/print.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(unsigned int);
extern void keyboard_stub();
extern void* isr_stub_table[32];

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

// called from isr_common_stub (idt_asm.asm) for any of vectors 0-31.
// Never returns - a CPU exception this early means we can't safely
// continue, so just report it and halt.
void isr_exception_handler(unsigned int vector, unsigned int error_code)
{
    print_text("\nUnhandled CPU exception: ");
    print_uint(vector);
    print_text(" (error code: ");
    print_uint(error_code);
    print_text(")\nSystem halted.\n");

    asm volatile("cli");
    while (1) {
        asm volatile("hlt");
    }
}

void idt_init()
{
    for (int i = 0; i < 256; i++) {
        idt[i].base_low = 0;
        idt[i].base_high = 0;
        idt[i].selector = 0;
        idt[i].flags = 0;
        idt[i].zero = 0;
    }

    // register the generic handler for every CPU exception vector
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (unsigned int)isr_stub_table[i], 0x08, 0x8E);
    }

    // keyboard IRQ1 (remapped to vector 33) - overrides nothing above,
    // vector 33 is outside the 0-31 exception range
    idt_set_gate(33, (unsigned int)keyboard_stub, 0x08, 0x8E);

    idtp.limit = sizeof(idt) - 1;
    idtp.base = (unsigned int)&idt;

    // IDT must be loaded via lidt BEFORE any 'int' instruction is executed,
    // otherwise the CPU faults with a stale/invalid IDTR -> triple fault.
    idt_load((unsigned int)&idtp);
}