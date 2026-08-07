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
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
extern interrupt_handler

isr0:
    push dword 0
    push dword 0

    jmp interrupt_common

isr1:
    push dword 0
    push dword 1

    jmp interrupt_common

isr2:
    push dword 0
    push dword 2

    jmp interrupt_common

isr3:
    push dword 0
    push dword 3

    jmp interrupt_common

isr4:
    push dword 0
    push dword 4

    jmp interrupt_common

isr5:
    push dword 0
    push dword 5

    jmp interrupt_common

isr6:
    push dword 0
    push dword 6

    jmp interrupt_common

isr7:
    push dword 0
    push dword 7

    jmp interrupt_common

isr8:
    push dword 8

    jmp interrupt_common

isr9:
    push dword 0
    push dword 9

    jmp interrupt_common

isr10:
    push dword 10

    jmp interrupt_common

isr11:
    push dword 11

    jmp interrupt_common

isr12:
    push dword 12

    jmp interrupt_common

isr13:
    push dword 13

    jmp interrupt_common


isr14:
    push dword 14

    jmp interrupt_common

isr15:
    push dword 0
    push dword 15

    jmp interrupt_common

isr16:
    push dword 0
    push dword 16

    jmp interrupt_common

isr17:
    push dword 17

    jmp interrupt_common

isr18:
    push dword 0
    push dword 18

    jmp interrupt_common

isr19:
    push dword 0
    push dword 19

    jmp interrupt_common

isr20:
    push dword 0
    push dword 20

    jmp interrupt_common

isr21:
    push dword 0
    push dword 21

    jmp interrupt_common

isr22:
    push dword 0
    push dword 22

    jmp interrupt_common

isr23:
    push dword 0
    push dword 23

    jmp interrupt_common

isr24:
    push dword 0
    push dword 24

    jmp interrupt_common

isr25:
    push dword 0
    push dword 25

    jmp interrupt_common

isr26:
    push dword 0
    push dword 26

    jmp interrupt_common

isr27:
    push dword 0
    push dword 27

    jmp interrupt_common

isr28:
    push dword 0
    push dword 28

    jmp interrupt_common

isr29:
    push dword 0
    push dword 29

    jmp interrupt_common

isr30:
    push dword 0
    push dword 30

    jmp interrupt_common

isr31:
    push dword 0
    push dword 31

    jmp interrupt_common

irq0:
    push dword 0
    push dword 32

    jmp interrupt_common

irq1:
    push dword 0
    push dword 33

    jmp interrupt_common

irq2:
    push dword 0
    push dword 34

    jmp interrupt_common

irq3:
    push dword 0
    push dword 35

    jmp interrupt_common

irq4:
    push dword 0
    push dword 36

    jmp interrupt_common

irq5:
    push dword 0
    push dword 37

    jmp interrupt_common

irq6:
    push dword 0
    push dword 38

    jmp interrupt_common

irq7:
    push dword 0
    push dword 39

    jmp interrupt_common

irq8:
    push dword 0
    push dword 40

    jmp interrupt_common

irq9:
    push dword 0
    push dword 41

    jmp interrupt_common

irq10:
    push dword 0
    push dword 42

    jmp interrupt_common

irq11:
    push dword 0
    push dword 43

    jmp interrupt_common

irq12:
    push dword 0
    push dword 44

    jmp interrupt_common

irq13:
    push dword 0
    push dword 45

    jmp interrupt_common

irq14:
    push dword 0
    push dword 46

    jmp interrupt_common

irq15:
    push dword 0
    push dword 47

    jmp interrupt_common

interrupt_common:
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
