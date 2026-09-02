[bits 32]

section .multiboot
align 8

mb2_header_start:
    dd 0xE85250D6                           ; Magic number (Multiboot 2)
    dd 0                                    ; Architecture 0 (i386 32-bit protected mode)
    dd mb2_header_end - mb2_header_start    ; Header length
    ; Checksum
    dd -(0xE85250D6 + 0 + (mb2_header_end - mb2_header_start))

    ; Mandatory End Tag
    dw 0                                    ; Type = 0
    dw 0                                    ; Flags = 0
    dd 8                                    ; Size = 8
mb2_header_end:

section .bss
align 16
stack_bottom:
    resb 16384                              ; Reserve 16 KB for kernel stack
stack_top:

section .text.start

[extern main]
[global _start]

_start:
    cli

    ; Setup stack
    mov esp, stack_top
    mov ebp, esp

    ; Push EBX (contains physical address of Multiboot info structure)
    push ebx
    call main

.hang:
    cli
    hlt
    jmp .hang