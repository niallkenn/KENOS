#include "memorymap.h"

mmap_entry_t mmap_entries[MMAP_MAX_ENTRIES];
uint16_t mmap_count = 0;

void init_mmap(uint32_t mmap_addr, uint32_t mmap_length) {
    uint32_t offset = 0;
    mmap_count = 0;

    mmap_entry_t* entry = (mmap_entry_t*)mmap_addr;

    while (offset < mmap_length && mmap_count < MMAP_MAX_ENTRIES) {
        uint32_t entry_size = *(uint32_t*)(mmap_addr + offset);
        grub__mmap_entry_t* raw = (grub__mmap_entry_t*)(mmap_addr + offset + 4);

        mmap_entries[mmap_count].base = ((uint64_t)raw->base_high << 32) | raw->base_low;
        mmap_entries[mmap_count].length = ((uint64_t)raw->length_high << 32) | raw->length_low;
        mmap_entries[mmap_count].type = raw->type;
        mmap_count++;

        offset += entry_size + 4;
    }
}