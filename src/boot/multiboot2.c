#include "multiboot2.h"
#include "user/stdlib.h"

struct mmap_entry mmap_type1_entries[MAX_MMAP_ENTRIES] = {};
int mmap_type1_count = 0;

void parse_multiboot2(uint32_t multiboot_info_address) {
    // main multiboot info
    struct multiboot2_info_header* mbi = (struct multiboot2_info_header*)(uintptr_t)multiboot_info_address;

    // first multiboot tag
    struct multiboot2_tag* tag = (struct multiboot2_tag*)(mbi + 1);

    // loop through all tags defined in header until end tag
    while (tag->type != 0) {
        switch (tag->type) {
            case 6: {
                // mmap tag
                struct multiboot2_tag_6_mmap* mmap = (struct multiboot2_tag_6_mmap*)tag;

                // track start and end of entries
                uintptr_t entries_start = (uintptr_t)mmap->entries;
                uintptr_t entries_end = (uintptr_t)mmap + mmap->size;

                struct mmap_entry* entry = (struct mmap_entry*)entries_start;

                mmap_type1_count = 0;

                // loop through entries
                while ((uintptr_t)entry < entries_end) {
                    if (entry->type == 1) {
                        mmap_type1_entries[mmap_type1_count++] = *entry;
                    }

                    entry = (struct mmap_entry*)((uintptr_t)entry + mmap->entry_size);
                }
            }
        }

        // align up
        uint32_t aligned_size = (tag->size + 7) & ~7;
        // move to next tag
        tag = (struct multiboot2_tag*)((uintptr_t)tag + aligned_size);
    }
}