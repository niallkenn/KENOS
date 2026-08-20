#include "frameallocator.h"

uint8_t FrameAllocator::m_bitmap[FrameAllocator::MAX_BITMAP_SIZE] = {};
size_t FrameAllocator::m_next_free_index = 0;
size_t FrameAllocator::m_total_frames = 0;
size_t FrameAllocator::m_used_frames = 0;
size_t FrameAllocator::m_free_frames = 0;

bool FrameAllocator::is_frame_set(uint32_t frame) {
    return m_bitmap[frame / 8] & (1 << (frame % 8));
}

void FrameAllocator::set_frame(uint32_t frame) {
    if (is_frame_set(frame)) return;
    m_bitmap[frame / 8] |= (1 << (frame % 8));
    m_used_frames++;
    if (m_free_frames!=0) m_free_frames--;
}

void FrameAllocator::clear_frame(uint32_t frame) {
    if (!is_frame_set(frame)) return;
    m_bitmap[frame / 8] &= ~(1 << (frame % 8));
    m_free_frames++;
    if (m_used_frames!=0) m_used_frames--;
}

void FrameAllocator::reserve_region(uint64_t start, uint64_t end) {
    uint32_t start_frame = start / 4096;
    uint32_t end_frame = (end + 4095) / 4096;
    for (uint32_t i = start_frame; i < end_frame; i++) set_frame(i);
}

void FrameAllocator::free_region(uint64_t start, uint64_t end) {
    uint32_t start_frame = (start + 4095) / 4096; // Round start UP to be safe
    uint32_t end_frame = end / 4096;              // Round end DOWN to be safe
    
    for (uint32_t i = start_frame; i < end_frame; i++) {
        if (i >= m_total_frames) break;
        clear_frame(i);
    }
}

void FrameAllocator::bump_index() {
    if (m_next_free_index >= m_total_frames) {
        m_next_free_index = 0;
    } else m_next_free_index++;
}

void FrameAllocator::initialise() {
    uint64_t total_memory_bytes = 0;
    for (size_t i = 0; i < MemoryMap::count(); i++) {
        MemoryMapEntry& e = MemoryMap::entries()[i];

        if (e.type != 1) continue;

        uint64_t entry_end = e.base + e.length;

        if (entry_end > total_memory_bytes) total_memory_bytes = entry_end;
    }

    m_total_frames = (total_memory_bytes + PAGE_SIZE - 1) / PAGE_SIZE;
    m_used_frames = m_total_frames;
    m_free_frames = 0;

    for (size_t i = 0; i < FrameAllocator::MAX_BITMAP_SIZE; i++) m_bitmap[i] = 0b11111111;

    for (size_t i = 0; i < MemoryMap::count(); i++) {
        MemoryMapEntry& e = MemoryMap::entries()[i];
        
        if (e.type == 1) {
            uint32_t frame_start = e.base;
            uint32_t frame_end = e.base + e.length;

            free_region(frame_start, frame_end);
        }
    }

    uint32_t kernel_end_addr = reinterpret_cast<uint32_t>(_kernel_end); 

    reserve_region(0, kernel_end_addr);
}

void* FrameAllocator::allocate() {
    size_t start = m_next_free_index;

    do {
        size_t byte_index = m_next_free_index / 8;
        size_t bit_index = m_next_free_index % 8;

        if (!(m_bitmap[byte_index] & (1 << bit_index))) {
            set_frame(m_next_free_index);

            void* alloc_ptr = reinterpret_cast<void*>(m_next_free_index * PAGE_SIZE);


            bump_index();

            return alloc_ptr;
        }

        bump_index();

    } while (m_next_free_index != start);

    return nullptr;
}

void FrameAllocator::free(void* ptr) {
    if (ptr == nullptr) return;

    uint32_t frame = reinterpret_cast<uint32_t>(ptr);
    size_t frame_number = frame / PAGE_SIZE;

    if (frame_number >= m_total_frames) return;

    clear_frame(frame_number);
}