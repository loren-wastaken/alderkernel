#include "syscall.h"
#include "../headers/print.h"

static void sys_panic(void)
{
    print_text("\n*** KERNEL PANIC (syscall 0x10) ***\n");

    asm volatile("cli");
    while (1) {
        asm volatile("hlt");
    }
}

// deliberately triggers a real hardware #DE (divide error, vector 0),
// to prove the exception path in idt.c actually works end-to-end.
static void sys_test_divzero(void)
{
    print_text("Forcing a real divide-by-zero (#DE, vector 0)...\n");

    asm volatile(
        "xor %%edx, %%edx\n"
        "mov $1, %%eax\n"
        "mov $0, %%ecx\n"
        "div %%ecx\n"
        :
        :
        : "eax", "ecx", "edx"
    );

    // unreachable - isr_exception_handler (idt.c) halts the CPU first
}

void syscall_dispatch(unsigned int num)
{
    switch (num) {
        case SYS_READ:
            print_text("sys_read: not implemented yet (no VFS-backed reads).\n");
            break;

        case SYS_WRITE:
            print_text("sys_write: not implemented yet (no VFS-backed writes).\n");
            break;

        case SYS_REQUEST_RAM:
            print_text("sys_request_ram: not implemented yet (heap allocator not ready).\n");
            break;

        case SYS_UNKNOWN_03:
            print_text("syscall 0x03: reserved, not defined yet.\n");
            break;

        case SYS_KERNEL_PANIC:
            sys_panic();
            break;

        case SYS_TEST_DIVZERO:
            sys_test_divzero();
            break;

        default:
            print_text("Unknown syscall number: ");
            print_uint(num);
            print_text("\n");
            break;
    }
}