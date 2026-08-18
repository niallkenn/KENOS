#pragma once

#include "definitions.h"
#include "memorymap.h"

#define PAGE_SIZE 4096

class FrameAllocator {
    private:
        static constexpr size_t MAX_BITMAP_SIZE = 262144;
        static uint8_t m_bitmap[MAX_BITMAP_SIZE];
        static size_t m_next_free_index;

        static bool is_frame_set(uint32_t frame);
        static void set_frame(uint32_t frame);
        static void clear_frame(uint32_t frame);
        static void reserve_region(uint64_t start, uint64_t end);
        static void free_region(uint64_t start, uint64_t end);
        static void bump_index();

    public:
        FrameAllocator() = delete;

        static size_t m_used_frames;
        static size_t m_free_frames;
        static size_t m_total_frames;

        static void initialise();

        static void* allocate();
        static void free(void* ptr);

};