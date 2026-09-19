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

// task state segment struct
typedef struct tss_entry_t {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

// global gdt, holds gdt entries that describe kernel vs user code and data, and tss
extern gdt_entry_t gdt[6];
// global gdt pointer
extern gdt_ptr_t gdt_ptr;
// global tss
extern tss_entry_t tss_entry;

// initialise global descriptor table
void init_gdt();
#endif