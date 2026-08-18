#include "heapallocator.h"
#include "frameallocator.h"
#include "paging.h"

Block* HeapAllocator::m_head = nullptr;
uintptr_t HeapAllocator::m_heap_start = HeapAllocator::HEAP_START;
size_t HeapAllocator::m_heap_size = HeapAllocator::INITIAL_HEAP_SIZE;

void HeapAllocator::initialise() {
    size_t page_count = (INITIAL_HEAP_SIZE + 4095) / 4096;

    for (int i = 0; i < page_count; i++) {
        uint32_t frame = reinterpret_cast<uint32_t>(FrameAllocator::allocate());
        Paging::map_page(HEAP_START + i * 0x1000, frame, 0x3);
    }

    Block* first_block = reinterpret_cast<Block*>(m_heap_start);
    first_block->free = true;
    first_block->next = nullptr;
    first_block->size = m_heap_size - sizeof(Block);

    m_head = first_block;
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
            }
            
            current->free = false;

            break;
        }

    } while (current != nullptr);

    if (current == nullptr) return nullptr;

    return reinterpret_cast<void*>(current + 1);
}