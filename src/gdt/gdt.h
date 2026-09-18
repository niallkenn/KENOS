#ifndef GDTH
#define GDTH

#include "definitions.h"

// gdt entry struct definition
typedef struct gdt_entry_t {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

// gdt pointer struct
typedef struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

// global gdt, holds gdt entries that describe kernel vs user code and data
extern gdt_entry_t gdt[5];
// global gdt pointer
extern gdt_ptr_t gdt_ptr;

// initialise global descriptor table
void init_gdt();
#endif