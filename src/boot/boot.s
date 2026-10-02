# Constants for multiboot header
.set ALIGN, 1<<0
.set MEM_INFO, 1<<1
.set VIDEO_MODE, 1<<2
.set FLAGS, ALIGN | MEM_INFO | VIDEO_MODE
.set MAGIC, 0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

# Multiboot header declaration, marks the program as a kernel
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.long 0
.long 0
.long 0
.long 0
.long 0

.long 0
.long 1024
.long 768
.long 32

# Set up stack
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

# Kernel entry point, _start
.section .text
.global _start
.type _start, @function
_start:
    # Set stack pointer
    mov $stack_top, %esp

    # push multiboot info and the magic value, ebx and eax
    push %ebx
    push %eax

    # Call kernel main
    call kernel_main

    # System infinite loop if the computer has nothing else to do
    cli
1:  hlt
    jmp 1b

# Set size of the start symbol
.size _start, . - _start
 