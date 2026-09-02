ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    _kernel_start = .;

    .multiboot : ALIGN(4)
    {
        KEEP(*(.multiboot))
    }

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