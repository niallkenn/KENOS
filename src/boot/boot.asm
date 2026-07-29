[org 0x7c00]

mov ah, 0xe
mov bx, string

printString:                        ; loop through string and print each char,
    mov al, [bx]                    ; until last char is terminator
    cmp al, 0
    je next
    int 0x10
    inc bx
    jmp printString

next:
    mov ah, 0
    int 0x16
    mov ah, 0xe
    int 0x10
    jmp next

string:
    db "Hello world!", 10, 13, 0

char:
    db 0

jmp $                           ; jump to current instruction (idk why or what this does)
times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature