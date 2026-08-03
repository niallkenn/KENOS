#pragma once

#include "definitions.h"

class MemoryAllocator
{
private:
    MemoryAllocator() = default;

    static constexpr size_t ALIGNMENT = 8;

    static uint8_t heap[256 * 1024];

    struct BlockHeader
    {
        size_t size;
        BlockHeader *next;
    };

    static BlockHeader *free_list_head;

public:
    static void *kmalloc(size_t size) noexcept;
    static void kfree(void *ptr) noexcept;
    static void initialise() noexcept;
    static size_t alignup(size_t size) noexcept;
};

void *operator new(size_t size) noexcept;
void *operator new[](size_t size) noexcept;
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
void operator delete(void *ptr, size_t size) noexcept;
void operator delete[](void *ptr, size_t size) noexcept;