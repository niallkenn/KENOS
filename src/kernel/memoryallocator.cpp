#include "memoryallocator.h"

alignas(MemoryAllocator::alignment) uint8_t MemoryAllocator::m_heap_arena[MemoryAllocator::arena_size] = {0};
MemoryAllocator::BlockHeader* MemoryAllocator::m_free_list_head = nullptr;
bool MemoryAllocator::m_initialized = false;

void* MemoryAllocator::malloc(size_t size) noexcept {
    if (!m_initialized) {
        initialize();
    }

    if (size == 0) size = 1;

    size = align_up(size);

    BlockHeader* prev = nullptr;
    BlockHeader* curr = m_free_list_head;

    while (curr != nullptr) {
        if (curr->size >= size) {
            constexpr size_t minimum_split_size = sizeof(BlockHeader) + alignment;

            if (curr->size >= size + minimum_split_size) {
                uint8_t* curr_bytes = reinterpret_cast<uint8_t*>(curr);
                BlockHeader* leftover_block = reinterpret_cast<BlockHeader*>(curr);

                leftover_block->size = curr->size - size - sizeof(BlockHeader);
                leftover_block->next = curr->next;

                curr->size = size;

                if (prev == nullptr) {
                    m_free_list_head = leftover_block;
                } else {
                    prev->next = leftover_block;
                }
            } else {
                if (prev == nullptr) {
                    m_free_list_head = curr->next;
                } else {
                    prev->next = curr->next;
                }
            }

            return reinterpret_cast<void*>(curr + 1);
        }

        prev = curr;
        curr = curr->next;
    }

    return nullptr;
}

void MemoryAllocator::free(void* ptr) noexcept {
    if (ptr == nullptr) return;

    BlockHeader* block_to_free = reinterpret_cast<BlockHeader*>(ptr) - 1;

    if (m_free_list_head == nullptr) {
        block_to_free->next = nullptr;
        m_free_list_head = block_to_free;
        return;
    }

    BlockHeader* prev = nullptr;
    BlockHeader* curr = m_free_list_head;

    while (curr != nullptr && curr < block_to_free) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == nullptr) {
        block_to_free->next = m_free_list_head;
        m_free_list_head = block_to_free;
    } else {
        block_to_free->next = curr;
        prev->next = block_to_free;
    }

    coalesce();
}

size_t MemoryAllocator::align_up(size_t size) noexcept {
    return (size + alignment - 1) & ~(alignment - 1);
}

void MemoryAllocator::initialize() noexcept {
    m_free_list_head = reinterpret_cast<BlockHeader*>(m_heap_arena);
    m_free_list_head->size = arena_size - sizeof(BlockHeader);
    m_free_list_head->next = nullptr;
    m_initialized = true;
}

void MemoryAllocator::coalesce() noexcept {
    BlockHeader* curr = m_free_list_head;

    while (curr != nullptr && curr->next != nullptr) {
        uint8_t* curr_bytes = reinterpret_cast<uint8_t*>(curr);
        uint8_t* curr_end = curr_bytes + sizeof(BlockHeader) + curr->size;

        if (curr_end == reinterpret_cast<uint8_t*>(curr->next)) {
            curr->size += sizeof(BlockHeader) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void* operator new(size_t size) noexcept {
    return MemoryAllocator::malloc(size);
}

void* operator new[](size_t size) noexcept {
    return MemoryAllocator::malloc(size);
}

void operator delete(void* ptr) noexcept {
    MemoryAllocator::free(ptr);
}

void operator delete[](void* ptr) noexcept {
    MemoryAllocator::free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    MemoryAllocator::free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    MemoryAllocator::free(ptr);
}