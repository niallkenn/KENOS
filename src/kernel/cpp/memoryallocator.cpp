#include "memoryallocator.h"

uint8_t MemoryAllocator::heap[256 * 1024];
MemoryAllocator::BlockHeader *MemoryAllocator::free_list_head = nullptr;

void MemoryAllocator::initialise() noexcept
{
    free_list_head = reinterpret_cast<BlockHeader *>(heap);
    free_list_head->size = 256 * 1024 - sizeof(BlockHeader);
    free_list_head->next = nullptr;
}

size_t MemoryAllocator::alignup(size_t size) noexcept
{
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

void *MemoryAllocator::kmalloc(size_t size) noexcept
{
    size = alignup(size);

    if (!free_list_head)
    {
        initialise();
    }

    BlockHeader *previous = nullptr;
    BlockHeader *current = free_list_head;

    while (current != nullptr && current->size < size)
    {
        previous = current;
        current = current->next;
    }

    if (current == nullptr)
    {
        return nullptr;
    }

    if (current->size >= size + sizeof(BlockHeader) + ALIGNMENT) {
        BlockHeader *new_header = reinterpret_cast<BlockHeader *>(reinterpret_cast<uint8_t *>(current + 1) + size);
        new_header->size = current->size - size - sizeof(BlockHeader);
        new_header->next = current->next;

        if (previous == nullptr) {
            free_list_head = new_header;
        } else {
            previous->next = new_header;
        }

        current->size = size;

    } else {
        if (previous == nullptr) {
            free_list_head = current->next;
        } else {
            previous->next = current->next;
        }
    }

    return reinterpret_cast<void *>(current + 1);
}

void MemoryAllocator::kfree(void *ptr) noexcept
{
    if (!ptr) return;

    BlockHeader* block = reinterpret_cast<BlockHeader*>(ptr) - 1;

    BlockHeader *previous = nullptr;
    BlockHeader *current = free_list_head;

    uintptr_t block_address = reinterpret_cast<uintptr_t>(block);
    uintptr_t current_address = reinterpret_cast<uintptr_t>(current);
    uintptr_t previous_address = reinterpret_cast<uintptr_t>(previous);

    while (current != nullptr) {
        if (previous_address < block_address && block_address < current_address) break;
        else {
            previous = current;
            current = current->next;
        }
    }

    if (previous == nullptr) {
        free_list_head = block;
    } else {
        previous->next = block;
    }
    
    block->next = current;

    if (reinterpret_cast<uint8_t*>(block + 1) + block->size == reinterpret_cast<uint8_t*>(block->next)) {
        block->size += sizeof(BlockHeader) + block->next->size;
        block->next = block->next->next;
    }

    if (reinterpret_cast<uint8_t*>(block) == reinterpret_cast<uint8_t*>(previous + 1) + previous->size) {
        previous->size += sizeof(BlockHeader) + block->size;
        previous->next = block->next;
    }

    return;
}

void *operator new(size_t size) noexcept
{
    return MemoryAllocator::kmalloc(size);
}

void *operator new[](size_t size) noexcept
{
    return MemoryAllocator::kmalloc(size);
}

void operator delete(void *ptr) noexcept
{
    return MemoryAllocator::kfree(ptr);
}

void operator delete[](void *ptr) noexcept
{
    return MemoryAllocator::kfree(ptr);
}

void operator delete(void *ptr, size_t size) noexcept
{
    (void)size;
    return MemoryAllocator::kfree(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept
{
    (void)size;
    return MemoryAllocator::kfree(ptr);
}
