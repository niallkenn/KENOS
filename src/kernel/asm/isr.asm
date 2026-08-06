[global isr0]
[global isr14]
[extern interrupt_handler]

isr0:
    push dword 0
    push dword 0

    jmp isr_common

isr14:
    push dword 14

    jmp isr_common

isr_common:
    pusha

    push ds
    push es
    push fs
    push gs

    mov eax, esp
    push eax

    call interrupt_handler

    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa

    add esp, 8

    iret
