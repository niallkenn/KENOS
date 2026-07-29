mov ah, 0xe
mov al, 'a'
INT 0x10

jmp $                           ;jump to current instruction (idk why or what this does)
times 510-($-$$) db 0           ;fill sector
db 0x55, 0xaa                   ;add bootloader signature