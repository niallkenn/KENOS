#ifndef GDT_H
#define GDT_H

#include "user/definitions.h"

void init_gdt(void);

// gdt descriptor, kcode kdata ucode udata tss
struct __attribute__((packed)) gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
};

// pointer to pass to asm, for lgdt
struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint32_t base;
};

extern struct gdt_entry gdt[6];

#endif