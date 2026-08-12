[org 0x7c00]                    ; tell nasm bios loads at address 0x7c00
[bits 16]                       ; start cpu in 16 bit real mode

%include "build/kernel_info.asm"

KERNEL_LOAD_ADDRESS equ 0x1000        ; kernel entry start point
MEMORY_MAP_ENTRY_COUNT equ 0x8C00

.bootloader_start:
    mov [BOOT_DRIVE_NUMBER], dl ; save boot drive number passed to dl by bios

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

    mov ax, 0x0013
    int 0x10

    ; --- NEW: Get the 8x8 BIOS font pointer ---
    push es                     ; Save ES register
    mov ax, 0x1130              ; BIOS function: Get Font Information
    mov bh, 3                   ; BH = 3 requests the 8x8 font pointer
    int 0x10                    ; Returns segment in ES, offset in BP
    
    mov [FONT_SEGMENT], es      ; Save the segment
    mov [FONT_OFFSET], bp       ; Save the offset
    pop es                      ; Restore ES register

    ; memory map

    call do_e820
    mov [MEMORY_MAP_ENTRY_COUNT], si

    call load_kernel
    call switch_pm
    jmp $                       ; keep jmp here (do not excecute functions past here)

do_e820:
    mov di, 0x8000
    xor ebx, ebx
    xor si, si
    mov edx, 0x0534D4150
first_entry:
    mov eax, 0xE820
    mov ecx, 24
    int 0x15

    jc failed_e820
    mov edx, 0x0534D4150
    cmp eax, edx
    jne failed_e820
    jmp handle_entry
loop_e820:
    mov eax, 0xE820
    mov edx, 0x534D4150
    mov ecx, 24

    int 0x15

    jc done_e820

    cmp eax, 0x534D4150
    jne failed_e820

    add di, 24
    inc si
    
    cmp ebx, 0
    jne loop_e820
    jmp done_e820
handle_entry:
    add di, 24
    inc si
    
    cmp ebx, 0
    jne loop_e820
    jmp done_e820
failed_e820:
    stc
    ret
done_e820:
    clc
    ret
load_kernel:
    mov ah, 2
    mov al, KERNEL_SECTORS
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [BOOT_DRIVE_NUMBER]
    mov bx, KERNEL_LOAD_ADDRESS 

    int 0x13

    ; handle disk read errors
    jc handle_disk_error    
    cmp al, KERNEL_SECTORS
    jne handle_disk_error
    ret

handle_disk_error:
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
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    jmp KERNEL_LOAD_ADDRESS

times 400-($-$$) db 0

FONT_OFFSET:       dw 0         ; Stores font offset pointer
FONT_SEGMENT:      dw 0
BOOT_DRIVE_NUMBER: db 0

times 510-($-$$) db 0           ; fill sector
db 0x55, 0xaa                   ; add bootloader signature