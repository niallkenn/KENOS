mov ah, 0xe                         ; print first character, a
mov al, 'a'
int 0x10                            

print:                              ; prints alphabet using loop,
    inc al                          ; the loop increment the character until
    cmp al, 'z' + 1                 ; it is past z, then exits
    je end
    int 0x10
    jmp print

end:
    jmp $                           ; jump to current instruction (idk why or what this does)
    times 510-($-$$) db 0           ; fill sector
    db 0x55, 0xaa                   ; add bootloader signature