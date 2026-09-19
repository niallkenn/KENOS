#include "gdt.h"

// init
gdt_entry_t gdt[6] = {};
gdt_ptr_t gdt_ptr;
tss_entry_t tss_entry;

// asm function that reloads code segment and call lgdt
extern void gdt_flush(uint32_t gdt_ptr_address);
// asm function that loads task state segment
extern void tss_flush();

// set gate definition
static void set_gate(int vector, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[vector].base_low = base & 0xFFFF;
    gdt[vector].base_middle = (base >> 16) & 0xFF;
    gdt[vector].base_high = (base >> 24) & 0xFF;

    gdt[vector].limit_low = limit & 0xFFFF;
    gdt[vector].granularity = (limit >> 16) & 0x0F;
    gdt[vector].granularity |= granularity & 0xF0;
    gdt[vector].access = access;
}

// init gdt definition
void init_gdt() {
    // init gdt ptr
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    // init tss entry
    unsigned char* p = (unsigned char*)&tss_entry;
    int num = sizeof(tss_entry);
    while (num--) {
        *p++ = 0;
    }

    tss_entry.ss0 = 0x10;
    tss_entry.esp0 = 0x0;
    uint32_t tss_base = (uint32_t)&tss_entry;
    uint32_t tss_limit = sizeof(tss_entry) - 1;
    
    // null descriptor needed in x86 arch 
    set_gate(0, 0, 0, 0, 0);

    // kernel code segment
    set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // kernel data segment
    set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // user code segment
    set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // user code segment
    set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // task state segment
    set_gate(5, tss_base, tss_limit, 0xE9, 0x00);

    // pass gdt pointer to asm wrapper
    gdt_flush((uint32_t)&gdt_ptr);

    // call ltr
    tss_flush();
}