#include "frameallocator.h"
#include "memorymap.h"

static uint8_t bitmap[MAX_BITMAP_SIZE] = {};
static size_t next_free_index = 0;

size_t fa_used_frames = 0;
size_t fa_free_frames = 0;
size_t fa_total_frames = 0;

static bool is_frame_set(uint32_t frame) {
    return bitmap[frame / 8] & (1 << (frame % 8));
}

static void set_frame(uint32_t frame) {
    if (is_frame_set(frame)) return;

    bitmap[frame / 8] |= (1 << (frame % 8));

    fa_used_frames++;
    if (fa_free_frames != 0) fa_free_frames--;
}

static void clear_frame(uint32_t frame) {
    if (!is_frame_set(frame)) return;

    bitmap[frame / 8] &= ~(1 << (frame % 8));

    fa_free_frames++;
    if (fa_used_frames != 0) fa_used_frames--;
}

static void reserve_region(uint64_t start, uint64_t end) {
    uint32_t start_frame = start / PAGE_SIZE;
    uint32_t end_frame = (end + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint32_t i = start_frame; i < end_frame; i++) set_frame(i);
}

static void free_region(uint64_t start, uint64_t end) {
    uint32_t start_frame = (start + PAGE_SIZE - 1) / PAGE_SIZE;
    uint32_t end_frame = end / PAGE_SIZE;

    for (uint32_t i = start_frame; i < end_frame; i++) {
        if (i >= fa_total_frames) break;

        clear_frame(i);
    }
}

static void bump_index(void) {
    if (next_free_index >= fa_total_frames) {
        next_free_index = 0;
    } else {
        next_free_index++;
    }
}

void init_fa(void) {
    uint64_t total_memory_bytes = 0;
    for (size_t i = 0; i < mmap_count; i++) {
        mmap_entry_t entry = mmap_entries[i];

        if (entry.type != 1) continue;

        uint64_t entry_end = entry.base + entry.length;

        if (entry_end > total_memory_bytes) total_memory_bytes = entry_end;
    }

    fa_total_frames = (total_memory_bytes + PAGE_SIZE - 1) / PAGE_SIZE;
    fa_used_frames = fa_total_frames;
    fa_free_frames = 0;

    for (size_t i = 0; i < MAX_BITMAP_SIZE; i++) bitmap[i] = 0b11111111;

    for (size_t i = 0; i < mmap_count; i++) {
        mmap_entry_t entry = mmap_entries[i];

        if (entry.type == 1) {
            free_region(entry.base, entry.base + entry.length);
        }
    }

    uint32_t kernel_end_address = (uint32_t)_kernel_end;

    reserve_region(0, kernel_end_address);
}

void* fa_allocate(void) {
    size_t start = next_free_index;

    do {
        size_t byte_index = next_free_index / 8;
        size_t bit_index = next_free_index % 8;

        if (!(bitmap[byte_index] & (1 << bit_index))) {
            set_frame(next_free_index);

            void* alloc_ptr = (void*)(uintptr_t)(next_free_index * PAGE_SIZE);

            bump_index();
            return alloc_ptr;
        }

        bump_index();
    } while (next_free_index != start);

    return NULL;
}

void fa_free(void* ptr) {
    if (ptr == NULL) return;

    uint32_t frame = (uint32_t)(uintptr_t)ptr;
    size_t frame_number = frame / PAGE_SIZE;

    if (frame_number >= fa_total_frames) return;

    clear_frame(frame_number);
}