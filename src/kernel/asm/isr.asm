global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
extern interrupt_handler

isr0:
    push dword 0
    push dword 0

    jmp isr_common

isr1:
    push dword 0
    push dword 1

    jmp isr_common

isr2:
    push dword 0
    push dword 2

    jmp isr_common

isr3:
    push dword 0
    push dword 3

    jmp isr_common

isr4:
    push dword 0
    push dword 4

    jmp isr_common

isr5:
    push dword 0
    push dword 5

    jmp isr_common

isr6:
    push dword 0
    push dword 6

    jmp isr_common

isr7:
    push dword 0
    push dword 7

    jmp isr_common

isr8:
    push dword 8

    jmp isr_common

isr9:
    push dword 0
    push dword 9

    jmp isr_common

isr10:
    push dword 10

    jmp isr_common

isr11:
    push dword 11

    jmp isr_common

isr12:
    push dword 12

    jmp isr_common

isr13:
    push dword 13

    jmp isr_common


isr14:
    push dword 14

    jmp isr_common

isr15:
    push dword 0
    push dword 15

    jmp isr_common

isr16:
    push dword 0
    push dword 16

    jmp isr_common

isr17:
    push dword 17

    jmp isr_common

isr18:
    push dword 0
    push dword 18

    jmp isr_common

isr19:
    push dword 0
    push dword 19

    jmp isr_common

isr20:
    push dword 0
    push dword 20

    jmp isr_common

isr21:
    push dword 0
    push dword 21

    jmp isr_common

isr22:
    push dword 0
    push dword 22

    jmp isr_common

isr23:
    push dword 0
    push dword 23

    jmp isr_common

isr24:
    push dword 0
    push dword 24

    jmp isr_common

isr25:
    push dword 0
    push dword 25

    jmp isr_common

isr26:
    push dword 0
    push dword 26

    jmp isr_common

isr27:
    push dword 0
    push dword 27

    jmp isr_common

isr28:
    push dword 0
    push dword 28

    jmp isr_common

isr29:
    push dword 0
    push dword 29

    jmp isr_common

isr30:
    push dword 0
    push dword 30

    jmp isr_common

isr31:
    push dword 0
    push dword 31

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
