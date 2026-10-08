#include "user/definitions.h"
#include "user/stdlib.h"
#include "boot/multiboot2.h"
#include "arch/i386/sys/gdt.h"
#include "arch/i386/sys/tss.h"

// main kernel c entry, ring 0
void kernel_main(uint32_t magic, uint32_t multiboot_info_address) {
    (void)magic;

    // interpret multiboot info
    parse_multiboot2(multiboot_info_address);
    
    // init gdt, sort segment registers, load in gdt structure
    init_gdt();

    // init the task state segment. loads task register with tss selector, initialises ss0 and esp0 in the structure.
    init_tss();
    
    // load global descriptor table
    for (;;) asm volatile("cli; hlt");
}