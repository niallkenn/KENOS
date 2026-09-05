ENTRY(_start)

SECTIONS
{
    /* Physical entry point at 1MB */
    . = 0x00100000;

    .boot ALIGN(4) : AT(0x00100000)
    {
        *(.boot)
    }

    /* Transition location counter to Higher Half (3GB) */
    . += 0xC0000000;

    _kernel_start = .;

    .text ALIGN(4096) : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text*)
    }

    .rodata ALIGN(4096) : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata*)
    }

    .data ALIGN(4096) : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data*)
    }

    .bss ALIGN(4096) : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss*)
    }

    _kernel_end = .;
}