.section .text

.extern interrupt_handler

# macro for the isr definitions, no error code
.macro ISR_NO_ERR_CODE n
    .global isr\n
isr\n:
    pushl $0 # fake error code
    pushl $\n # interrupt vector number
    jmp interrupt_common
.endm

# macro for the isr defintions, error code
.macro ISR_ERR_CODE n
    .global isr\n
isr\n:
              # already pushes error code
    pushl $\n # interrupt vector number
    jmp interrupt_common
.endm

# isr defintions using macros
ISR_NO_ERR_CODE 0
ISR_NO_ERR_CODE 1
ISR_NO_ERR_CODE 2
ISR_NO_ERR_CODE 3
ISR_NO_ERR_CODE 4
ISR_NO_ERR_CODE 5
ISR_NO_ERR_CODE 6
ISR_NO_ERR_CODE 7
ISR_ERR_CODE 8
ISR_NO_ERR_CODE 9
ISR_ERR_CODE 10
ISR_ERR_CODE 11
ISR_ERR_CODE 12
ISR_ERR_CODE 13
ISR_ERR_CODE 14
ISR_NO_ERR_CODE 15
ISR_NO_ERR_CODE 16
ISR_ERR_CODE 17
ISR_NO_ERR_CODE 18
ISR_NO_ERR_CODE 19
ISR_NO_ERR_CODE 20
ISR_ERR_CODE 21
ISR_NO_ERR_CODE 22
ISR_NO_ERR_CODE 23
ISR_NO_ERR_CODE 24
ISR_NO_ERR_CODE 25
ISR_NO_ERR_CODE 26
ISR_NO_ERR_CODE 27
ISR_NO_ERR_CODE 28
ISR_NO_ERR_CODE 29
ISR_NO_ERR_CODE 30
ISR_NO_ERR_CODE 31

# common cleanup for interrupt handlers
interrupt_common:
    pushal

    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es

    mov %esp, %eax
    pushl %eax

    call interrupt_handler

    add $4, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds

    popal

    add $8, %esp

    iret
