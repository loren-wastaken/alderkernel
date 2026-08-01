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
    ; Don't trust GRUB's leftover GDT/selectors - load our own known-good
    ; flat GDT so selector 0x08/0x10 used by idt_set_gate() are guaranteed
    ; to be a valid flat code/data segment.
    lgdt [gdt_descriptor]

    ; far jump to reload CS with our code selector (0x08) and flush
    ; the stale GRUB code segment out of the pipeline
    jmp 0x08:.reload_segments

.reload_segments:
    mov ax, 0x10        ; our data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; just setup a safe stack pointer in bss section
    mov esp, stack_top   

    ; execute kernel code
    call kernel_start   

_loop:
    hlt
    jmp _loop

section .data
align 8
gdt_start:
    dq 0x0000000000000000          ; null descriptor

gdt_code:
    dw 0xFFFF                      ; limit low
    dw 0x0000                      ; base low
    db 0x00                        ; base middle
    db 10011010b                   ; access: present, ring0, code, exec/read
    db 11001111b                   ; flags(4KB gran, 32-bit) + limit high
    db 0x00                        ; base high

gdt_data:
    dw 0xFFFF                      ; limit low
    dw 0x0000                      ; base low
    db 0x00                        ; base middle
    db 10010010b                   ; access: present, ring0, data, read/write
    db 11001111b                   ; flags(4KB gran, 32-bit) + limit high
    db 0x00                        ; base high

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1     ; GDT limit
    dd gdt_start                   ; GDT base address

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB of safe stack space
stack_top:
