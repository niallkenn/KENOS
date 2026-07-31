[org 0x7c00]                    ; tell nasm bios loads at address 0x7c00
[bits 16]                       ; start cpu in 16 bit real mode

KERNEL_OFFSET equ 0x1000        ; kernel entry start point

.bootloader_start:
    mov [BOOT_DRIVE_NUMBER], dl ; save boot drive number passed to dl by bios

    cli                         ; disable interrupts for setup

    ; ensure registers for tiny memory model
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; setup stack
    mov bp, 0x8000
    mov sp, bp

    sti                         ; re-enable interrupts

    mov ax, 0x0013
    int 0x10

    call load_kernel
    call switch_pm
    jmp $                       ; keep jmp here (do not excecute functions past here)

load_kernel:
    mov ah, 2
    mov al, 15
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [BOOT_DRIVE_NUMBER]
    mov bx, KERNEL_OFFSET

    int 0x13

    ; handle disk read errors
    jc handle_disk_error    
    cmp al, 15
    jne handle_disk_error

    ret

handle_disk_error:
    jmp $                       ; stop program after disk error

switch_pm:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEGMENT:start_protected_mode

GDT_START:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff               ; first 16 bits of the limit
        dw 0
        db 0                    ; first 24 / 32 bits of the base
        db 10011010b             ; present, privilege, type and type flags
        db 11001111b             ; other flags and last 4 bits of limit
        db 0                    ; last 8 bits of the base
    data_descriptor:
        dw 0xffff               ; first 16 bits of the limit
        dw 0
        db 0                    ; first 24 / 32 bits of the base
        db 10010010b             ; present, privilege, type and type flags
        db 11001111b             ; other flags and last 4 bits of limit
        db 0                    ; last 8 bits of the base
GDT_END:

gdt_descriptor:
    dw GDT_END - GDT_START - 1
    dd GDT_START

CODE_SEGMENT equ code_descriptor - GDT_START
DATA_SEGMENT equ data_descriptor - GDT_START


[bits 32]
start_protected_mode:
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    jmp KERNEL_OFFSET

BOOT_DRIVE_NUMBER: db 0

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature