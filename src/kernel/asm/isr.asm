[global isr0]
[extern interrupt_handler]

isr0:
    push 0

    call interrupt_handler

    add esp, 4

    iret