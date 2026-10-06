#include "definitions.h"
#include "multiboot2.h"

// main kernel c entry, ring 0
void kernel_main(uint32_t magic, uint32_t multiboot_info_address) {
    (void)magic;
    // interpret multiboot info
    parse_multiboot2(multiboot_info_address);

    volatile unsigned char* v = (volatile unsigned char*)0xb8000;
    char str[30];
    itoa(mmap_type1_entries[mmap_type1_count - 1].base, str);
    for (int i = 0; i < strlen(str); i++) {
        v[i * 2] = str[i];
    }
    for (;;) asm volatile("cli; hlt");
}