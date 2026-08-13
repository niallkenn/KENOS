ENTRY(.start)

SECTIONS
{
    /* Load the kernel safely at the 1MB mark to avoid BIOS data areas */
    . = 0x9000; 
    _kernel_start = .;

    /* Force every section to align to a clean 4KB page boundary */
    .text : ALIGN(4K) {
        *(.text.start)
        *(.text)
    }

    .rodata : ALIGN(4K) {
        *(.rodata*)
    }

    .data : ALIGN(4K) {
        *(.data)
    }

    .bss : ALIGN(4K) {
        *(.COMMON)
        *(.bss)
    }

    _kernel_end = .;
}
