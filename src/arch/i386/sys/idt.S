.section .text

# C interrupt handler
.extern interrupt_handler

.global load_idt
.type load_idt, @function

# loads the idt pointer into the idt 48 bit register
load_idt:
    # take struct pointer from stack, argument passed by C
    movl 4(%esp), %eax

    # loads it into idtr
    lidt (%eax)

    ret

# every interrupt calls this. handles pushing values along with cpu pushed to C, to handle interrupt.
interrupt_common:
    # pushes 32bit general purpose registers
    pushal

    # push segment registers, for changing when switching ring
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # must load kernel data selector for C code. now in ring 0
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    # now esp points to bottom of interrupt frame, so now have to push esp, the pointer to the frame
    pushl %esp

    call interrupt_handler

    # pop the struct pointer pushed
    addl $4, %esp

    # pop everything in reverse order
    popl %gs
    popl %fs
    popl %es
    popl %ds

    # pop genreal purpose registers
    popal

    # pop the vector and error code, 8 bytes for 2 32bit unsigned integers
    addl $8, %esp

    # iret returns from isr, and pops eip, cs, eflags, and useresp and ss if going to ring 3. good for switching pl
    iret

# macros for the isr stubs, different for yes/no pushed error codes by cpu
.macro ISR_NO_ERROR_CODE num
.global isr\num
isr\num:
    pushl $0 # push fake error code for interrupts that dont push one
    pushl $\num # push vector
    jmp interrupt_common
.endm

.macro ISR_ERROR_CODE num
.global isr\num
isr\num:
    pushl $\num # cpu pushes error code, push vector
    jmp interrupt_common
.endm

# isr for exceptions
ISR_NO_ERROR_CODE 0
ISR_NO_ERROR_CODE 1
ISR_NO_ERROR_CODE 2
ISR_NO_ERROR_CODE 3
ISR_NO_ERROR_CODE 4
ISR_NO_ERROR_CODE 5
ISR_NO_ERROR_CODE 6
ISR_NO_ERROR_CODE 7
ISR_ERROR_CODE 8
ISR_NO_ERROR_CODE 9
ISR_ERROR_CODE 10
ISR_ERROR_CODE 11
ISR_ERROR_CODE 12
ISR_ERROR_CODE 13
ISR_ERROR_CODE 14
ISR_NO_ERROR_CODE 15
ISR_NO_ERROR_CODE 16
ISR_ERROR_CODE 17
ISR_NO_ERROR_CODE 18
ISR_NO_ERROR_CODE 19
ISR_NO_ERROR_CODE 20
ISR_ERROR_CODE 21
ISR_NO_ERROR_CODE 22
ISR_NO_ERROR_CODE 23
ISR_NO_ERROR_CODE 24
ISR_NO_ERROR_CODE 25
ISR_NO_ERROR_CODE 26
ISR_NO_ERROR_CODE 27
ISR_NO_ERROR_CODE 28
ISR_NO_ERROR_CODE 29
ISR_NO_ERROR_CODE 30
ISR_NO_ERROR_CODE 31

# irqs
ISR_NO_ERROR_CODE 32 # irq 0, timer