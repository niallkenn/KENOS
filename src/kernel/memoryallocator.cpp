#include "memoryallocator.h"

uint8_t MemoryAllocator::heap[256 * 1024];
uint8_t* MemoryAllocator::next_free = heap;

void* MemoryAllocator::kmalloc(size_t size) noexcept {
    uint8_t* current = next_free;

    BlockHeader* header;
    header->size = size;

    current = reinterpret_cast<uint8_t*>(header);
    next_free+=size;
    return current + header->size;
}

void MemoryAllocator::kfree(void* ptr) noexcept {
    return;
}

void* operator new(size_t size) noexcept {
    return MemoryAllocator::kmalloc(size);
}

void* operator new[](size_t size) noexcept {
    return MemoryAllocator::kmalloc(size);
}

void operator delete(void* ptr) noexcept {
    return MemoryAllocator::kfree(ptr);
}

void operator delete[](void* ptr) noexcept {
    return MemoryAllocator::kfree(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    return MemoryAllocator::kfree(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    return MemoryAllocator::kfree(ptr);
}

