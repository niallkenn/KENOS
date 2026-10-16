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

// main kernel c entry, ring 0
void kernel_main(uint32_t magic, uint32_t multiboot_info_address) {
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

    // initialise programmable interval timer, 100 ticks per second, square mode
    init_pit(PIT_TICKS_PER_SECOND); // 100 tps

    // enable hardware interrupts after setup
    asm volatile("sti");

    volatile unsigned char* fb = (volatile unsigned char*)0xb8000;
    uint32_t index = 10;

    while (1) {
        pit_wait_ms(1);
        fb[index++ * 2] = '.';
    }

    // halt cpu, nothing else to do
    for (;;) asm volatile("cli; hlt");
}