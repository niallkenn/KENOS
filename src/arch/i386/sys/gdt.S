.section .text

.global load_gdt
.type load_gdt, @function

load_gdt:
    # first argument, the address of gdt_ptr, passed from c
    movl 4(%esp), %eax

    # load the gdt to register using lgdt
    lgdt (%eax)

    # load the data segment registers with the kernel data selector.
    # selector = (index * 8) | TI | RPL (index in gdt, ti = 0 for gdt, rpl is kernel(0), user(3))
    # kernel data selector = (2 * 8) | 0 | 0 = 0x10

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # to load code segment register cs, you need to far jump with code segment selector
    # kernel code segment selector = (1 * 8) | 0 | 0 = 0x08
    ljmp $0x08, $1f
1:
    # return from load_gdt
    ret