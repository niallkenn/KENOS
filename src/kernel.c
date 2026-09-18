// includes
#include "idt.h"

// main kernel function
void kernel_main() {
    // init interrupt descriptor table
    init_idt();

    // hold cpu
    while (1) asm volatile("hlt");
}