global idt_load
global keyboard_stub
global isr13

extern keyboard_handler


idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret

global isr13

isr13:
    cli

.loop:
    hlt
    jmp .loop

global isr3

isr3:
    cli

.loop:
    hlt
    jmp .loop

keyboard_stub:

    pusha

    call keyboard_handler

    popa

    mov al, 0x20
    out 0x20, al

    iretd