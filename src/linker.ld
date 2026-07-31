ENTRY(.start)

OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x1000;

    .text : {
        build/kernel_entry.o(.text)
        *(.text)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}