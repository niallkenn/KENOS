ENTRY(_start)

SECTIONS
{
    . = 0x9000;

    _kernel_start = .;

    .text ALIGN(4K) :
    {
        KEEP(*(.text.start))
        *(.text*)
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    }

    .data ALIGN(4K) :
    {
        *(.data*)
    }

    .bss ALIGN(4K) :
    {
        *(.bss*)
        *(COMMON)
    }

    _kernel_end = .;
}