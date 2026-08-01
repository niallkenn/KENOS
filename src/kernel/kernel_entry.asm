[bits 32]
[extern main]
[global .start]

section .text.start
.start:
    call main
    jmp $