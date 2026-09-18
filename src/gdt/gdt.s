.section .text
.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    /* Get GDT pointer passed as first argument on stack */
    movl 4(%esp), %eax

    /* Load the GDT register */
    lgdt (%eax)

    /* Reload data segment registers with kernel data selector 0x10 */
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    /* Far jump to reload CS with kernel code selector 0x08 */
    ljmp $0x08, $1f

1:
    ret
