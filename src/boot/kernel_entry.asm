[bits 32]

[extern main]
[global _start]

section .text.start

_start:
    call main
    cli
.hang:
    hlt
    jmp .hang