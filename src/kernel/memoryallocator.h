#pragma once

#include "definitions.h"

class MemoryAllocator {
    private:
        MemoryAllocator() = default;

        static uint8_t heap[256 * 1024];
        static uint8_t* next_free;

        struct BlockHeader {
            size_t size;
        };

    public:
        static void* kmalloc(size_t size) noexcept;
        static void kfree(void* ptr) noexcept;
};

void* operator new(size_t size) noexcept;
void* operator new[](size_t size) noexcept;
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, size_t size) noexcept;
void operator delete[](void* ptr, size_t size) noexcept;