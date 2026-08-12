#include "memorymap.h"

static MemoryMapEntry* map_entries = nullptr;
static uint16_t map_count = 0;

void MemoryMap::initialize() {
    map_entries = reinterpret_cast<MemoryMapEntry*>(0x8000);
    map_count = *reinterpret_cast<uint16_t*>(0x8C00);
}

MemoryMapEntry* MemoryMap::entries() { return map_entries; }

uint16_t MemoryMap::count() { return map_count; }