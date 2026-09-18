// includes
#include "idt.h"
#include "gdt.h"
// main kernel function
void kernel_main() {
    init_gdt();
    // init interrupt descriptor table
    init_idt();

    // hold cpu
    while (1) asm volatile("hlt");
}