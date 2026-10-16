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

void print_char(char c, uint32_t index) {
    volatile unsigned char* fb = (volatile unsigned char*)0xb8000;
    fb[index * 2] = c;
}

void print_mmap_entries(void) {
    volatile unsigned char* fb = (volatile unsigned char*)0xb8000;
    uint32_t index = 0;

    for (int i = 0; i < mmap_entry_count; i++) {
        char type[1];
        itoa(mmap_entries[i].type, type);
        print_char(type[0], index++);
        print_char(' ', index++);


        char base[20];
        itoa(mmap_entries[i].base, base);
        for (int i = 0; i < strlen(base); i++) print_char(base[i], index++);
        print_char(' ', index++);

        char length[20];
        itoa(mmap_entries[i].length, length);
        for (int i = 0; i < strlen(length); i++) print_char(length[i], index++);

        index += 10;
    }
}

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

    // initialise programmable interval timer, 1000 ticks per second, square mode
    init_pit(PIT_TICKS_PER_SECOND); // 1000 tps

    // enable hardware interrupts after setup
    asm volatile("sti");

    print_mmap_entries();

    // halt cpu, nothing else to do
    for (;;) asm volatile("cli; hlt");
}