.section .text
.global enter_usermode
.type enter_usermode, @function

# enter ring 3, user mode and iret
enter_usermode:
    cli

    # ebx = entry_point of user code
    # ecx = user_stack
    mov 4(%esp), %ebx
    mov 8(%esp), %ecx

    # user data segment selector | ring 3
    mov $0x23, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # user data segment
    pushl $0x23
    # user stack pointer
    pushl %ecx
    # eflags
    pushf

    # enable interrupts in reloaded eflags
    popl %eax
    orl $0x0200, %eax # enable interrupts in ring 3, sets bit 9
    pushl %eax

    # user code segment 0x18 | ring 3
    pushl $0x1B
    # eip, entry point address of user code
    pushl %ebx
    
    iret # pops eip, cs, eflags, esp, ss, reverse of pushed
