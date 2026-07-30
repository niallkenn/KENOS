[org 0x7c00]                    ; tell nasm bios loads at address 0x7c00
[bits 16]                       ; start cpu in 16 bit real mode

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature