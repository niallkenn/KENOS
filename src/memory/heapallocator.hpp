#pragma once

#include "definitions.hpp"

struct Block {
    size_t size;
    bool free;
    Block* next;
};

class HeapAllocator {
private:
    static uintptr_t m_heap_start;
    static size_t m_heap_size;
    
    static constexpr uintptr_t HEAP_START = 0xC0000000 + 0x00400000; 
    static constexpr size_t INITIAL_HEAP_SIZE = 256 * 1024; // 256 KB
    static constexpr size_t MINIMUM_BLOCK_SIZE = 8;
    
    HeapAllocator() = delete;
    
    static size_t align_up(size_t val, size_t alignment);
    
public:
    static Block* m_head;
    static size_t m_total_used;
    static size_t m_total_free;
    static size_t m_block_count;

    static void initialise();

    static void* kmalloc(size_t size);
    static void kfree(void* ptr);
};

void* kmalloc(size_t size);
void kfree(void* ptr);

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

void operator delete(void* ptr, size_t) noexcept;
void operator delete[](void* ptr, size_t) noexcept;