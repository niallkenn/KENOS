# multiboot header constants
.set MAGIC, 0xE85250D6
.set ARCHITECTURE, 0x0
.set CHECKSUM, -(MAGIC + ARCHITECTURE + (multiboot_end - multiboot_start))

# multiboot header
.section .multiboot2
.align 8
multiboot_start:
    .long MAGIC
    .long 0 # architecture , 0
    .long (multiboot_end - multiboot_start)
    .long CHECKSUM

    .align 8
    # tell multiboot what tags to pass
    info_request_tag_start:
        .short 1 # tag type 1, info req
        .short 0 # flags always 0
        .long info_request_tag_end - info_request_tag_start # size

        # requested tags
        .long 6 # mmap
    info_request_tag_end:

    .align 8
    # end tag
    .short 0
    .short 0
    .long 8
multiboot_end:

# initial kernel stack
.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text
.global _start
.type _start, @function

_start:
    # set stack pointer
    mov $stack_top, %esp

    # diable interrupts
    cli

    # eax = MAGIC
    # ebx = phys address of multiboot info structure

    push %ebx
    push %eax

    # call main c kernel
    call kernel_main

# loop
1:
    cli
    hlt
    jmp 1b