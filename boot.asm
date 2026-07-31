; multiboot header 4 grub spec
MAGIC    equ 0x1BADB002
FLAGS    equ 0x01 ; align loaded modules on page boundaries
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

[bits 32]

section .text
global _start
extern kernel_start ; main.c entry point

_start:
    ; grub already sets up perfect 32-bit segments 4 us
    ; just setup a safe stack pointer in bss section
    mov esp, stack_top   

    ; execute kernel code
    call kernel_start   

_loop:
    hlt
    jmp _loop

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB of safe stack space
stack_top:
