#include "pmm.h"
#include "boot/multiboot2.h"

// init
uint8_t reserved_bitmap[BITMAP_SIZE];
uint8_t allocated_bitmap[BITMAP_SIZE];

// align down and up inline definitions
static inline uint32_t align_down(uint32_t x, int align) {
    // bitwise magic
    return x & ~(align - 1);
}

static inline uint32_t align_up(uint32_t x, int align) {
    // crazy bitwise magic
    return (x + (align - 1)) & ~(align - 1);
}

static void set(uint32_t frame_index, uint8_t* bitmap) {
    // check index
    if (frame_index >= TOTAL_FRAMES) return;

    // byte of bitmap
    int byte = frame_index / 8;
    // bit of byte
    int bit = frame_index % 8;

    // sets bit, or with 1
    bitmap[byte] = bitmap[byte] | (1 << bit);
}

static void clear(uint32_t frame_index, uint8_t* bitmap) {
    // check index
    if (frame_index >= TOTAL_FRAMES) return;
    
    // byte of bitmap
    int byte = frame_index / 8;
    // bit of byte
    int bit = frame_index % 8;

    // clears bit, and with 0
    bitmap[byte] = bitmap[byte] & ~(1 << bit);
}

// sets a region a frames, takes aligned start and end frame
static void set_region(uint32_t start_frame, uint32_t end_frame, uint8_t* bitmap) {
    // loop through region
    while (start_frame <= end_frame) {
        // check if aligned
        if (start_frame % FRAME_SIZE != 0) return;

        // get frame index for single set
        uint32_t frame_index = start_frame / FRAME_SIZE;
        set(frame_index, bitmap);
        // increment to next frame
        start_frame += FRAME_SIZE;
    }
}

// clears a region a frames, takes aligned start and end frame
static void clear_region(uint32_t start_frame, uint32_t end_frame, uint8_t* bitmap) {
    // loop through region
    while (start_frame <= end_frame) {
        // check if aligned
        if (start_frame % FRAME_SIZE != 0) return;

        // get frame index for single clear
        uint32_t frame_index = start_frame / FRAME_SIZE;
        clear(frame_index, bitmap);
        // increment to next frame
        start_frame += FRAME_SIZE;
    }
}


void init_pmm(void) {
    // initialise bitmaps to all reserved and allocated
    for (int i = 0; i < BITMAP_SIZE; i++) {
        reserved_bitmap[i] = 0b11111111;
        allocated_bitmap[i] = 0b11111111;
    }

    // free up usable ram and mark not reserved
    for (int i = 0; i < mmap_entry_count; i++) {
        if (mmap_entries[i].type == 1) {
            uint32_t aligned_start = align_up(mmap_entries[i].base, FRAME_SIZE);
            uint32_t aligned_end = align_down(mmap_entries[i].base + mmap_entries[i].length, FRAME_SIZE);
            // mark allocated as no
            clear_region(aligned_start, aligned_end, allocated_bitmap);
            clear_region(aligned_start, aligned_end, reserved_bitmap);
        }
    }

    // reserve up to 1mb
    uint32_t mb1_end = align_up(0x100000, FRAME_SIZE);
    set_region(0, mb1_end, allocated_bitmap);
    set_region(0, mb1_end, reserved_bitmap);

    // reserve kernel image, _kernel_start to _kernel_end, includes all data and uninit data, multiboot info etc
    uint32_t align_kstart = align_down(_kernel_start, FRAME_SIZE);
    uint32_t align_kend = align_up(_kernel_end, FRAME_SIZE);
    set_region(align_kstart, align_kend, reserved_bitmap);
    set_region(align_kstart, align_kend, allocated_bitmap);
}