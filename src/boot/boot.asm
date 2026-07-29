[org 0x7c00]

string:
    db "Hello world!", 0

mov ah, 0xe
mov bx, string

printString:                        ; loop through string and print each char,
    mov al, [bx]                    ; until last char is terminator
    cmp al, 0
    je end
    int 0x10
    inc bx
    jmp printString

end:
    jmp $                           ; jump to current instruction (idk why or what this does)
    times 510-($-$$) db 0           ; fill sector
    db 0x55, 0xaa                   ; add bootloader signature