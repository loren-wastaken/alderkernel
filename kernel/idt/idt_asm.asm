global idt_load
global keyboard_stub

extern keyboard_handler


idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret


keyboard_stub:

    pusha

    call keyboard_handler

    popa

    iretd