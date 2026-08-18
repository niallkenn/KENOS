#pragma once

#include "definitions.h"

struct Block {
    size_t size;
    bool free;
    Block* next;
};

class HeapAllocator {
    private:
        static Block* m_head;
        static uintptr_t m_heap_start;
        static size_t m_heap_size;

        static constexpr uintptr_t HEAP_START = 0xC0000000;
        static constexpr size_t INITIAL_HEAP_SIZE = 256 * 1024;
        static constexpr size_t MINIMUM_BLOCK_SIZE = 8;

        HeapAllocator() = delete;

        static size_t align_up(size_t val, size_t alignment);

    public:
        static void initialise();

        static void* kmalloc(size_t size);
        static void kfree(void* ptr);
};  