[org 0x7c00]

mov bp, 0x8000                      ; initialise stack at 8000h
mov sp, bp

mov bx, buffer
jmp loop                            ; start program at loop

print_bx:                           ; prints bx
    pusha
    mov ah, 0xe
    mov al, 'b'
    int 0x10
    mov al, 'x'
    int 0x10
    mov al, 10
    int 0x10
    mov al, 13
    int 0x10
    popa
    ret

start_cycle:                        ; type-print loop
    mov ah, 0xe                     ; print new line after last print
    mov al, 10                  
    int 0x10
    mov al, 13
    int 0x10
    mov bx, buffer
loop:                               ; fill buffer with typed string
    mov ah, 0
    int 0x16
    cmp ah, 0xe
    jne call_print_bx
    call print_bx
call_print_bx:
    mov [bx], al
    mov ah, 0xe
    int 0x10
    inc bx
    cmp bx, buffer + 19
    ja printBuffer                  ; print buffer when its 20 char long
    jmp loop

printBuffer:                    
    mov ah, 0xe                     ; print new line after typing
    mov al, 10
    int 0x10
    mov al, 13
    int 0x10
    mov bx, buffer
printLoop:                          ; print string
    mov al, [bx]
    cmp al, 0
    je start_cycle
    int 0x10
    inc bx
    jmp printLoop

buffer:                             ; initialise buffer to 0
    times 21 db 0

end:
    jmp $                           ; jump to current instruction (idk why or what this does)
    times 510-($-$$) db 0           ; fill sector
    db 0x55, 0xaa                   ; add bootloader signature