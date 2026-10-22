#include "user/definitions.h"
#include "user/stdlib.h"
#include "boot/multiboot2.h"
#include "arch/i386/sys/gdt.h"
#include "arch/i386/sys/tss.h"
#include "arch/i386/sys/idt.h"
#include "arch/i386/sys/pic.h"
#include "arch/i386/sys/interrupts.h"
#include "arch/i386/sys/io.h"
#include "arch/i386/sys/pit.h"
#include "memory/pmm.h"

// main kernel c entry, ring 0
void kernel_main(uint32_t magic, uint32_t multiboot_info_address)
{
    (void)magic;

    // interpret multiboot info
    parse_multiboot2(multiboot_info_address);

    // init gdt, sort segment registers, load in gdt structure
    init_gdt();

    // init the task state segment. loads task register with tss selector, initialises ss0 and esp0 in the structure.
    init_tss();

    // init the interrupt descriptor table, loads the idt into the idtr, sets gates for interrupt service routines
    init_idt();

    // remap the pic to allow irqs, remove conflict with certain exception entries in idt
    init_pic();

    // initialise programmable interval timer, 1000 ticks per second, square mode
    init_pit(PIT_TICKS_PER_SECOND); // 1000 tps

    // initialise physical memory manager, reserving ram regions for physical frame allocation
    init_pmm();

    // enable hardware interrupts after setup
    asm volatile("sti");

    // vga framebuffer pointer
    volatile unsigned char *fb = (volatile unsigned char *)0xb8000;
    
    // allocation check
    for (int i = 0; i < 32408; i++) 
        if (frame_allocate() == 0) asm volatile("ud2");

    // log count
    char count[30];
    itoa(free_frame_count, count);

    for (int i = 0; i < strlen(count); i++)
        fb[i * 2] = count[i];

    // halt cpu, nothing else to do
    for (;;)
        asm volatile("cli; hlt");
}