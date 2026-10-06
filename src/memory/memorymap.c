#include "memorymap.h"

mmap_entry_t mmap_entries[MMAP_MAX_ENTRIES];
uint16_t mmap_count = 0;

void init_mmap(uint32_t mmap_addr, uint32_t mmap_length) {
    uint32_t offset = 0;
    mmap_count = 0;

    while (offset + sizeof(uint32_t) <= mmap_length &&
           mmap_count < MMAP_MAX_ENTRIES) {
        uint32_t entry_size = *(uint32_t *)(uintptr_t)(mmap_addr + offset);

        /*
         * GRUB's mmap entry size is the size of the entry after the
         * size field itself. Reject malformed entries so a bad map
         * cannot make us loop forever or read past the supplied buffer.
         */
        if (entry_size < sizeof(grub__mmap_entry_t) ||
            entry_size > mmap_length - offset - sizeof(uint32_t)) {
            break;
        }

        grub__mmap_entry_t *raw =
            (grub__mmap_entry_t *)(uintptr_t)(mmap_addr + offset + sizeof(uint32_t));

        mmap_entries[mmap_count].base =
            ((uint64_t)raw->base_high << 32) | raw->base_low;
        mmap_entries[mmap_count].length =
            ((uint64_t)raw->length_high << 32) | raw->length_low;
        mmap_entries[mmap_count].type = raw->type;
        mmap_count++;

        offset += sizeof(uint32_t) + entry_size;
    }
}
