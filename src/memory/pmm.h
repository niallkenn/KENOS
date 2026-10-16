#ifndef PMM_H
#define PMM_H

#include "user/definitions.h"

// global macros
#define FRAME_SIZE 4096
#define TOTAL_ACCESSIBLE_MEMORY (4ULL * 1024 * 1024 * 1024)
#define TOTAL_FRAMES (TOTAL_ACCESSIBLE_MEMORY / FRAME_SIZE)
#define BITMAP_SIZE (TOTAL_FRAMES / 8) // 1 byte entries in bitmap

// global bitmap definitions
extern uint8_t reserved_bitmap[BITMAP_SIZE];
extern uint8_t allocated_bitmap[BITMAP_SIZE];

// init frame allocator
void init_pmm(void);

// frame allocator allocate and free
uint32_t frame_allocate(void);
bool frame_free(uint32_t frame);

#endif