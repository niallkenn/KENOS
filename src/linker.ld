# Entry point for kernel 
ENTRY(_start)

# declares where object files and data goes in kernel image

SECTIONS
{
    # sets start location for kernel
    . = 1M;

    # global kernel start
    _kernel_start = .;

    # put multiboot header and text / code section first
    .text : ALIGN(4K)
    {
        KEEP(*(.multiboot2))
        *(.text)
    }

    # read only data
    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    # read write data 
    .data : ALIGN(4K)
    {
        *(.data)
    }

    # read write data uninitialised and stack
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    # global kernel end
    _kernel_end = .;
}