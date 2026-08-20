#include "heapallocator.h"
#include "frameallocator.h"
#include "paging.h"
#include "panic.h"

Block* HeapAllocator::m_head = nullptr;
uintptr_t HeapAllocator::m_heap_start = HeapAllocator::HEAP_START;
size_t HeapAllocator::m_heap_size = HeapAllocator::INITIAL_HEAP_SIZE;
size_t HeapAllocator::m_total_used = 0;
size_t HeapAllocator::m_total_free = HeapAllocator::INITIAL_HEAP_SIZE;
size_t HeapAllocator::m_block_count = 0;

void HeapAllocator::initialise() {
    size_t page_count = (INITIAL_HEAP_SIZE + 4095) / 4096;

    for (size_t i = 0; i < page_count; i++) {
        uint32_t frame = reinterpret_cast<uint32_t>(FrameAllocator::allocate());
        Paging::map_page(HEAP_START + i * 0x1000, frame, 0x3);
    }

    Block* first_block = reinterpret_cast<Block*>(m_heap_start);
    first_block->free = true;
    first_block->next = nullptr;
    first_block->size = m_heap_size - sizeof(Block);

    m_head = first_block;

    m_block_count = 1;
    m_total_free = m_heap_size;
    m_total_used = 0;
}

size_t HeapAllocator::align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

void* HeapAllocator::kmalloc(size_t size) {
    if (m_head == nullptr || size == 0) return nullptr;

    size = align_up(size, 8);

    Block* current = m_head;

    do {
        if (current->free == false) {
            current = current->next;
            continue;
        } else if (current->size < size) {
            current = current->next;
            continue;
        } else {
            if (current->size >= size + sizeof(Block) + MINIMUM_BLOCK_SIZE) {
                Block* new_block = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(current + 1) + size);

                new_block->size = current->size - size - sizeof(Block);
                new_block->free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;

                m_block_count++;
            }
            
            current->free = false;

            break;
        }

    } while (current != nullptr);

    if (current == nullptr) return nullptr;

    m_total_used += size;
    m_total_free = m_heap_size - m_total_used;



    return reinterpret_cast<void*>(current + 1);
}

void HeapAllocator::kfree(void* ptr) {
    if (ptr == nullptr) return;

    Block* free_block = reinterpret_cast<Block*>(ptr) - 1;

    size_t freed_size = free_block->size;
    
    free_block->free = true;

    if (free_block->next != nullptr && free_block->next->free == true) {
        free_block->size += sizeof(Block) + free_block->next->size;
        free_block->next = free_block->next->next;
        m_block_count--;
    }

    Block* previous = m_head;

    if (free_block != m_head) {
        while (previous->next != free_block) {
        previous = previous->next;
        }

        if (previous->free) {
            previous->size += sizeof(Block) + previous->next->size;
            previous->next = previous->next->next;
            m_block_count--;
        }
    }

    m_total_used -= freed_size;
    m_total_free = m_heap_size - m_total_used;
}

void* kmalloc(size_t size) {
    void* ptr = HeapAllocator::kmalloc(size);

    if (ptr == nullptr) {
        panic("kmalloc: allocation failed");
    }

    return ptr;
}

void kfree(void* ptr) {
    HeapAllocator::kfree(ptr);
}

void* operator new(size_t size) {
    return kmalloc(size);
}

void* operator new[](size_t size) {
    return kmalloc(size);
}

void operator delete(void* ptr) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr) noexcept {
    kfree(ptr);
}

void operator delete(void* ptr, size_t) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr, size_t) noexcept {
    kfree(ptr);
}