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
    call switch_pm
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

    jne handle_disk_error

    ret

handle_disk_error:
    mov si, DISK_ERROR_MSG
    call print_string_16
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
    mov edi, 0xb8000
    mov al, ' '
    mov ah, 0xf0
    mov [0xb8000], ax
    mov ecx, 2000

    .loopclear:
        mov [edi], ax
        add edi, 2
        loop .loopclear


BOOT_DRIVE_NUMBER: db 0
REAL_MODE_MSG: db "Started in 16-bit real mode...", 13, 10, 0
LOAD_KERNEL_MSG: db "Loading kernel into RAM at 0x1000...", 13, 10, 0
DISK_ERROR_MSG: db "Disk read failed!", 13, 10, 0

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature