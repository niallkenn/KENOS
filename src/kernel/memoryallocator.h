#pragma once

#include "definitions.h"

class MemoryAllocator {
    private:
        MemoryAllocator() = default;
        uint8_t heap[256 * 1024];

    public:
        static void* kmalloc(size_t size);
        static void kfree(void* ptr);
};