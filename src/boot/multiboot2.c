#include "multiboot2.h"
#include "stdlib.h"

void parse_multiboot2(uint32_t multiboot_info_address) {
    // main multiboot info
    struct multiboot2_info_header* mbi = (struct multiboot2_info_header*)(uintptr_t)multiboot_info_address;

    // first multiboot tag
    struct multiboot2_tag* tag = (struct multiboot2_tag*)(mbi + 1);

    // loop through all tags defined in header until end tag
    while (tag->type != 0) {
        switch (tag->type) {
            case 6: {
                // TODO parse into memory map entries tag and make usable
            }
        }

        uint32_t aligned_size = (tag->size + 7) & ~7;
        tag = (struct multiboot2_tag*)((uintptr_t)tag + aligned_size);
    }
}