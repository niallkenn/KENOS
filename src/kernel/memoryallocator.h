#pragma once
#include <cstddef>
#include "definitions.h"
class MemoryAllocator {
    public:
        MemoryAllocator() = delete;
        ~MemoryAllocator() = delete;
        MemoryAllocator(const MemoryAllocator&) = delete;
        MemoryAllocator& operator=(const MemoryAllocator&) = delete;

        static void malloc(size_t size) noexcept;
        static void free(void* ptr) noexcept;
    
    private:
        struct BlockHeader {
            size_t size;
            BlockHeader* next;
        };

        static constexpr size_t arena_size = 256 * 1024;
        static constexpr size_t alignment = alignof(max_align_t);

        static alignas(alignment) uint8_t m_heap_arena[arena_size];
        static BlockHeader* m_free_list_head;
        static bool m_initialized;

        static size_t align_up(size_t size) noexcept;
        static void initialize() noexcept;
        static void coalesce() noexcept;
};

void* operator new(size_t size) noexcept;
void* operator new(size_t size) noexcept;

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

void operator delete(void* ptr, std::size_t size) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;