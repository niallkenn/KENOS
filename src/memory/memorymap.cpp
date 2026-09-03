#include "memorymap.hpp"

static MemoryMapEntry* map_entries = nullptr;
static uint16_t map_count = 0;

struct __attribute__((packed)) MultibootTag {
    uint32_t type;
    uint32_t size;
};

struct __attribute__((packed)) MultibootMemoryMapTag {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
};

void MemoryMap::initialise(uint32_t address) {
    map_entries = nullptr;
    map_count = 0;

    uint8_t* info = reinterpret_cast<uint8_t*>(address);

    MultibootTag* tag = reinterpret_cast<MultibootTag*>(info + 8);

    while (tag->type != 0) {
        if (tag->type == 6) {
            MultibootMemoryMapTag* mmap = reinterpret_cast<MultibootMemoryMapTag*>(tag);

            uint8_t* entry_address = reinterpret_cast<uint8_t*>(mmap) + sizeof(MultibootMemoryMapTag);

            uint32_t entry_count = (mmap->size - sizeof(MultibootMemoryMapTag)) / mmap->entry_size;

            map_entries = reinterpret_cast<MemoryMapEntry*>(0x8000);
            map_count = 0;

            for (uint32_t i = 0; i < entry_count; i++) {
                MemoryMapEntry* raw_entry = reinterpret_cast<MemoryMapEntry*>(entry_address + (i * mmap->entry_size));

                map_entries[map_count] = *raw_entry;
                map_count++;
            }

            return;
        }

        uint32_t next = (tag->size + 7) & ~7;

        tag = reinterpret_cast<MultibootTag*>(reinterpret_cast<uint8_t*>(tag) + next);
    }
}

MemoryMapEntry* MemoryMap::entries() { return map_entries; }

uint16_t MemoryMap::count() { return map_count; }