#ifndef MB2_H
#define MB2_H

#include "definitions.h"

#define MAX_MMAP_ENTRIES 256

// actual base header of entire multiboot2 info structure
struct __attribute__((packed)) multiboot2_info_header {
    uint32_t total_size;
    uint32_t reserved;
};

// multiboot tags. all have a uint32 type and uint32 size
// general tag
struct __attribute__((packed)) multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

// memory map entry, multiboot passes variable number of these as descriptors in mmap tag
struct __attribute__((packed)) mmap_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
};

// tag 6, mmap
struct __attribute__((packed)) multiboot2_tag_6_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct mmap_entry entries[MAX_MMAP_ENTRIES];
};

// global mmap type 1 array and count
extern struct mmap_entry mmap_type1_entries[MAX_MMAP_ENTRIES];
extern int mmap_type1_count;

// parse multiboot header
void parse_multiboot2(uint32_t multiboot_info_address);

#endif