.section .text

.global start_first_process
.type start_first_process, @function

start_first_process:
    mov 4(%esp), %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds

    popa

    addl $8, %esp

    iret
