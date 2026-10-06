#ifndef MEMORYMAPH
#define MEMORYMAPH

#include "definitions.h"

#define MMAP_MAX_ENTRIES 64

typedef struct mmap_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
} mmap_entry_t;

typedef struct {
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} __attribute__((packed)) grub__mmap_entry_t;

extern mmap_entry_t mmap_entries[MMAP_MAX_ENTRIES];
extern uint16_t mmap_count;

void init_mmap(uint32_t mmap_addr, uint32_t mmap_length);

#endif
