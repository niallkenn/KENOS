#include "frameallocator.h"
#include "memorymap.h"

static uint8_t bitmap[MAX_BITMAP_SIZE];
static size_t next_free_index = 0;

size_t fa_used_frames = 0;
size_t fa_free_frames = 0;
size_t fa_total_frames = 0;

static bool valid_frame(size_t frame) {
    return frame < fa_total_frames && frame < MAX_MANAGED_FRAMES;
}

static bool is_frame_set(size_t frame) {
    if (!valid_frame(frame)) return true;
    return (bitmap[frame / 8] & (uint8_t)(1u << (frame % 8))) != 0;
}

static void set_frame(size_t frame) {
    if (!valid_frame(frame) || is_frame_set(frame)) return;

    bitmap[frame / 8] |= (uint8_t)(1u << (frame % 8));
    fa_used_frames++;
    if (fa_free_frames > 0) fa_free_frames--;
}

static void clear_frame(size_t frame) {
    if (!valid_frame(frame) || !is_frame_set(frame)) return;

    bitmap[frame / 8] &= (uint8_t)~(1u << (frame % 8));
    fa_free_frames++;
    if (fa_used_frames > 0) fa_used_frames--;
}

static void reserve_region(uint64_t start, uint64_t end) {
    if (end <= start) return;

    uint64_t start_frame = start / PAGE_SIZE;
    uint64_t end_frame = (end + PAGE_SIZE - 1) / PAGE_SIZE;

    if (start_frame >= fa_total_frames) return;
    if (end_frame > fa_total_frames) end_frame = fa_total_frames;

    for (size_t i = (size_t)start_frame; i < (size_t)end_frame; i++) {
        set_frame(i);
    }
}

static void free_region(uint64_t start, uint64_t end) {
    if (end <= start) return;

    uint64_t start_frame = (start + PAGE_SIZE - 1) / PAGE_SIZE;
    uint64_t end_frame = end / PAGE_SIZE;

    if (start_frame >= fa_total_frames) return;
    if (end_frame > fa_total_frames) end_frame = fa_total_frames;

    for (size_t i = (size_t)start_frame; i < (size_t)end_frame; i++) {
        clear_frame(i);
    }
}

static void bump_index(void) {
    if (fa_total_frames == 0) {
        next_free_index = 0;
        return;
    }

    next_free_index++;
    if (next_free_index >= fa_total_frames) {
        next_free_index = 0;
    }
}

void init_fa(void) {
    uint64_t total_memory_bytes = 0;

    for (size_t i = 0; i < mmap_count; i++) {
        mmap_entry_t entry = mmap_entries[i];

        if (entry.type != 1 || entry.length == 0) continue;

        uint64_t entry_end = entry.base + entry.length;
        if (entry_end < entry.base) entry_end = UINT64_MAX;

        if (entry_end > total_memory_bytes)
            total_memory_bytes = entry_end;
    }

    uint64_t frame_count =
        (total_memory_bytes + PAGE_SIZE - 1) / PAGE_SIZE;

    /*
     * The bitmap is fixed-size. Never allow the allocator to address
     * beyond it.
     */
    if (frame_count > MAX_MANAGED_FRAMES)
        frame_count = MAX_MANAGED_FRAMES;

    fa_total_frames = (size_t)frame_count;
    fa_used_frames = fa_total_frames;
    fa_free_frames = 0;
    next_free_index = 0;

    for (size_t i = 0; i < MAX_BITMAP_SIZE; i++)
        bitmap[i] = 0xFF;

    /*
     * Only pages explicitly reported as usable by the firmware/GRUB
     * memory map are made available.
     */
    for (size_t i = 0; i < mmap_count; i++) {
        mmap_entry_t entry = mmap_entries[i];

        if (entry.type == 1)
            free_region(entry.base, entry.base + entry.length);
    }

    /*
     * Keep the kernel and everything below _kernel_end unavailable.
     * _kernel_end must be a physical/identity-mapped linker symbol
     * in this 32-bit paging layout.
     */
    uint32_t kernel_end_address = (uint32_t)(uintptr_t)_kernel_end;
    reserve_region(0, kernel_end_address);
}

void *fa_allocate(void) {
    if (fa_free_frames == 0 || fa_total_frames == 0)
        return NULL;

    size_t start = next_free_index;

    do {
        if (!is_frame_set(next_free_index)) {
            set_frame(next_free_index);

            void *alloc_ptr =
                (void *)(uintptr_t)(next_free_index * PAGE_SIZE);

            bump_index();
            return alloc_ptr;
        }

        bump_index();
    } while (next_free_index != start);

    return NULL;
}

void fa_free(void *ptr) {
    if (ptr == NULL) return;

    uintptr_t address = (uintptr_t)ptr;

    /* A frame allocator only accepts page-aligned physical addresses. */
    if ((address & (PAGE_SIZE - 1)) != 0) return;

    size_t frame_number = address / PAGE_SIZE;
    clear_frame(frame_number);
}
