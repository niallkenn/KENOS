#include "definitions.h"
#include "multiboot2.h"

// main kernel c entry, ring 0
void kernel_main(uint32_t magic, uint32_t multiboot_info_address) {
    // interpret multiboot info
    parse_multiboot2(multiboot_info_address);
    for (;;) asm volatile("cli; hlt");
}