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