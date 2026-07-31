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

    mov si, REAL_MODE_MSG
    call print_string_16

    jmp $                       ; keep jmp here (do not excecute functions past here)

print_string_16:
    pusha
    mov ah, 0x0e
    loop:
        mov al, [si]
        inc si
        cmp al, 0
        je done
        int 0x10
        jmp loop
    done:
        popa
        ret

REAL_MODE_MSG: db "Started in 16-bit real mode...", 13, 10, 0

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature