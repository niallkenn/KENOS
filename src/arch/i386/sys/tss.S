.section .text

.global init_tss_asm
.type init_tss_asm, @function

# init_tss_asm loads the tss selector into the task register using the ltr op
init_tss_asm:
    # the selector is the (index * 8) | ti | dpl, index = 5, ti = dpl = 0, selector = 40 = 0x28
    mov $0x28, %ax
    ltr %ax
    
    ret