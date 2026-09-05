[bits 32]

; --- MULTIBOOT 1 HEADER ---
MB_ALIGN     equ  1 << 0
MB_MEMINFO   equ  1 << 1
MB_FLAGS     equ  MB_ALIGN | MB_MEMINFO
MB_MAGIC     equ  0x1BADB002
MB_CHECKSUM  equ -(MB_MAGIC + MB_FLAGS)

section .boot
align 4
header_start:
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

align 4096
boot_page_directory:
    ; PDE 0 (0x00000000): Identity map 0-4MB (Present + Writable + PSE 4MB = 0x83)
    dd 0x00000083
    times 767 dd 0

    ; PDE 768 (0xC0000000): Higher-half map 0-4MB -> Physical 0x00000000
    dd 0x00000083
    times 255 dd 0

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .boot
global _start
extern kernel_main

_start:
    ; 1. Enable PSE (4MB Pages) in CR4 FIRST
    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    ; 2. Load physical address of boot_page_directory into CR3
    mov ecx, (boot_page_directory - 0xC0000000)
    mov cr3, ecx

    ; 3. Enable Paging in CR0
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    ; 4. Jump to higher-half virtual address
    lea ecx, [higher_half]
    jmp ecx

section .text
higher_half:
    ; 5. Set stack pointer to Higher Half Virtual Address
    mov esp, stack_top

    ; 6. Print 'OK' directly to VGA buffer to verify execution
    mov word [0xC00B8000], 0x2F4F ; 'O' white on green
    mov word [0xC00B8002], 0x2F4B ; 'K' white on green

    ; 7. Infinite loop so it doesn't crash back to GRUB
.hang:
    hlt
    jmp .hang