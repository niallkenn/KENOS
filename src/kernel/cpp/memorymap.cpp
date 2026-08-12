#include "memorymap.h"

MemoryMapEntry* MemoryMap::map_entries = nullptr;
uint16_t MemoryMap::map_count = 0;

void MemoryMap::initialize() {
    map_entries = reinterpret_cast<MemoryMapEntry*>(0x8000);
    map_count = *reinterpret_cast<uint16_t*>(0x8C00);
}