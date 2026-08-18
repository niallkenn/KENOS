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

        HeapAllocator() = delete;

    public:
        static void initialise();

        static void* kmalloc(size_t size);
        static void kfree(void* ptr);
};  