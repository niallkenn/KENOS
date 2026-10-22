#include "pmm.h"
#include "boot/multiboot2.h"

// init
uint8_t reserved_bitmap[BITMAP_SIZE];
uint8_t allocated_bitmap[BITMAP_SIZE];
uint32_t allocated_frame_count;
uint32_t free_frame_count;

// align down and up inline definitions
static inline uint64_t align_down(uint64_t x, int align)
{
    // bitwise magic
    return x & ~(align - 1);
}

static inline uint64_t align_up(uint64_t x, int align)
{
    // crazy bitwise magic
    return (x + (align - 1)) & ~(align - 1);
}

static void set(uint32_t frame_index, uint8_t *bitmap)
{
    // check index
    if (frame_index >= TOTAL_FRAMES)
        return;

    // byte of bitmap
    int byte = frame_index / 8;
    // bit of byte
    int bit = frame_index % 8;

    // sets bit, or with 1
    bitmap[byte] = bitmap[byte] | (1 << bit);
}

static void clear(uint32_t frame_index, uint8_t *bitmap)
{
    // check index
    if (frame_index >= TOTAL_FRAMES)
        return;

    // byte of bitmap
    int byte = frame_index / 8;
    // bit of byte
    int bit = frame_index % 8;

    // clears bit, and with 0
    bitmap[byte] = bitmap[byte] & ~(1 << bit);
}

// sets a region a frames, takes aligned start and end frame
static void set_region(uint32_t start_addr, uint32_t end_addr, uint8_t *bitmap)
{
    // check if aligned
    if (start_addr % FRAME_SIZE != 0)
        return;

    if (end_addr % FRAME_SIZE != 0)
        return;

    // loop through region
    while (start_addr < end_addr)
    {
        // get frame index for single set
        uint32_t frame_index = start_addr / FRAME_SIZE;
        set(frame_index, bitmap);
        // increment to next frame
        start_addr += FRAME_SIZE;
    }
}

// clears a region a frames, takes aligned start and end frame
static void clear_region(uint32_t start_addr, uint32_t end_addr, uint8_t *bitmap)
{
    // check if aligned
    if (start_addr % FRAME_SIZE != 0)
        return;

    if (end_addr % FRAME_SIZE != 0)
        return;

    // loop through region
    while (start_addr < end_addr)
    {
        // get frame index for single clear
        uint32_t frame_index = start_addr / FRAME_SIZE;
        clear(frame_index, bitmap);
        // increment to next frame
        start_addr += FRAME_SIZE;
    }
}

void init_pmm(void)
{
    // initialise bitmaps to all reserved and allocated
    for (uint32_t i = 0; i < BITMAP_SIZE; i++)
    {
        reserved_bitmap[i] = 0b11111111;
        allocated_bitmap[i] = 0b11111111;
    }

    // free up usable ram and mark not reserved
    for (int i = 0; i < mmap_entry_count; i++)
    {
        if (mmap_entries[i].type == 1)
        {
            // physical memory limit
            uint64_t limit = TOTAL_FRAMES * FRAME_SIZE;
            // start and end of entry
            uint64_t start = mmap_entries[i].base;
            uint64_t end = mmap_entries[i].base + mmap_entries[i].length;

            // if its outside pmm range
            if (start >= limit) continue;

            // if start + length overflows
            if (end < start)
            {
                end = UINT64_MAX;
            }

            // clip the end to limit
            if (end > limit)
            {
                end = limit;
            }

            // align start and end
            start = align_up(start, FRAME_SIZE);
            end = align_down(end, FRAME_SIZE);

            // mark not allocated and not reserved
            clear_region(start, end, allocated_bitmap);
            clear_region(start, end, reserved_bitmap);
        }
    }

    // reserve up to 1mb
    uint32_t mb1_end = align_up(0x100000, FRAME_SIZE);
    set_region(0, mb1_end, allocated_bitmap);
    set_region(0, mb1_end, reserved_bitmap);

    // reserve kernel image, _kernel_start to _kernel_end, includes all data and uninit data, multiboot info etc
    uint32_t align_kstart = align_down((uint32_t)_kernel_start, FRAME_SIZE);
    uint32_t align_kend = align_up((uint32_t)_kernel_end, FRAME_SIZE);
    set_region(align_kstart, align_kend, reserved_bitmap);
    set_region(align_kstart, align_kend, allocated_bitmap);

    // init global variables
    allocated_frame_count = 0;
    free_frame_count = 0;

    // increment total free frames
    for (uint32_t i = 0; i < TOTAL_FRAMES; i++)
    {
        bool allocated = (allocated_bitmap[i / 8]) & (1 << (i % 8));
        bool reserved = (reserved_bitmap[i / 8]) & (1 << (i % 8));

        // is the frame free and not reserved
        if (!allocated && !reserved)
            free_frame_count++;
    }
}

uint32_t frame_allocate(void)
{
    // loop through bitmaps and check for frame that is free and not reserved
    for (uint32_t i = 0; i < TOTAL_FRAMES; i++)
    {
        // frame byte is i / 8, bit in byte is i % 8
        if (!(allocated_bitmap[i / 8] & (1 << (i % 8))) &&
            !(reserved_bitmap[i / 8] & (1 << (i % 8))))
        {
            // bit is clear in both bitmaps
            // set allocated
            allocated_bitmap[i / 8] |= (1 << (i % 8));

            // change free and allocated count
            if (free_frame_count > 0) free_frame_count--;
            allocated_frame_count++;

            // return frames address
            return (uint32_t)(i * FRAME_SIZE);
        }
    }

    // no frames are free and not reserved, return 0 for fail
    return 0;
}

// returns 0 on success of freeing frame, add error codes later
bool frame_free(uint32_t frame)
{
    // is frame 4k aligned
    if (frame % FRAME_SIZE != 0)
        return 1;

    // frame index for frame
    uint32_t frame_index = frame / FRAME_SIZE;

    // is frame within memory
    if (frame_index >= TOTAL_FRAMES)
        return 1;

    // check if frame is reserved, reject free
    if (reserved_bitmap[frame_index / 8] & (1 << (frame_index % 8)))
        return 1;

    // check if already free
    if (!(allocated_bitmap[frame_index / 8] & (1 << (frame_index % 8))))
        return 1;

    // set frame free
    clear(frame_index, allocated_bitmap);

    // update variable count
    free_frame_count++;
    if (allocated_frame_count > 0) allocated_frame_count--;

    // success
    return 0;
}