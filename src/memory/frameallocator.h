#ifndef FRAMEALLOCATORH
#define FRAMEALLOCATORH

#include "definitions.h"

#define PAGE_SIZE 4096
#define MAX_BITMAP_SIZE 262144

extern size_t fa_used_frames;
extern size_t fa_free_frames;
extern size_t fa_total_frames;

void init_fa(void);

void* fa_allocate(void);
void fa_free(void* ptr);

#endif