[org 0x7c00]                    ; tell nasm bios loads at address 0x7c00
[bits 16]                       ; start cpu in 16 bit real mode

KERNEL_OFFSET equ 0x1000        ; kernel entry start point

.bootloader_start:
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

    mov [BOOT_DRIVE_NUMBER], dl ; save boot drive number passed to dl by bios

    mov si, REAL_MODE_MSG
    call print_string_16

    call load_kernel

    jmp $                       ; keep jmp here (do not excecute functions past here)

print_string_16:
    pusha
    mov ah, 0x0e
    .loop:
        mov al, [si]
        inc si
        cmp al, 0
        je .done
        int 0x10
        jmp .loop
    .done:
        popa
        ret

load_kernel:
    mov si, LOAD_KERNEL_MSG
    call print_string_16

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

    je handle_disk_error

    jmp $

handle_disk_error:
    mov si, DISK_ERROR_MSG
    call print_string_16
    jmp $                       ; stop program after disk error

BOOT_DRIVE_NUMBER: db 0
REAL_MODE_MSG: db "Started in 16-bit real mode...", 13, 10, 0
LOAD_KERNEL_MSG: db "Loading kernel into RAM at 0x1000...", 13, 10, 0
DISK_ERROR_MSG: db "Disk read failed!", 13, 10, 0

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature