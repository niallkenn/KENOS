[org 0x7c00]                    ; tell nasm bios loads at address 0x7c00
[bits 16]                       ; start cpu in 16 bit real mode

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

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature