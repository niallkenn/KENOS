#pragma once

#include "definitions.h"

#define FREE 0
#define USED 1
#define PAGE_SIZE 4096

class FrameAllocator {
    private:
        uint8_t* bitmap;
        size_t next_free_index;
        
        size_t total_allocated;

        static bool is_frame_set(uint32_t frame);
        static void set_frame(uint32_t frame);
        static void clear_frame(uint32_t frame);
        static void reserve_region(uint32_t start, uint32_t end);
        static void bump_index();

    public:
        FrameAllocator() = delete;

        static void initialise();

        static void* allocate();
        static void free(void* ptr);

        size_t total_frames() { return total_allocated; }
};