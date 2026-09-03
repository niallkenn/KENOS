#pragma once

#include "definitions.hpp"

struct __attribute((packed)) MemoryMapEntry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t flags;
};

class MemoryMap {
    public:
        MemoryMap() = delete;

        static void initialize(uint32_t address);

        static MemoryMapEntry* entries();
        static uint16_t count();
};