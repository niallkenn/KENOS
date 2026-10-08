#include "gdt.h"
#include "tss.h"
// global gdt
struct gdt_entry gdt[6];

// asm defined lgdt and set up segment registers
extern void load_gdt(uint32_t gdt_ptr);

// set entry in gdt
static void set_gate(int index, uint32_t base, uint32_t limit, uint8_t granularity, uint8_t access) {
    gdt[index].base_low = base & 0xFFFF; // first 16 bits of base
    gdt[index].base_middle = (base >> 16) & 0xFF; // next 8 bits
    gdt[index].base_high = (base >> 24) & 0xFF; // last 8 bits

    gdt[index].limit_low = limit & 0xFFFF; // first 16 bits in limit

    gdt[index].granularity = (limit >> 16) & 0b1111; // upper 4 bits of limit go in lower 4 of granularity
    gdt[index].granularity |= granularity & 0b11110000; // actual flags for granularity, G, size (D/B) then access and avl, not used
    gdt[index].access = access; // present, dpl, descriptor type(excecutable, code/data), direction/conforming(based on E, stack?/code from dpl)
}

// initialise gdt and segment registers
void init_gdt() {
    // null descriptor
    set_gate(0, 0, 0, 0, 0);

    // kernel code segment granularity-G/32bit, access-present/ring0/code/excecutable/non-conforming (go by dpl)/readable/not accessed
    set_gate(1, 0, 0xFFFFFFFF, 0b11001111, 0b10011010);
    // kernel data segment granularity-G/32bit, access-present/ring0/data/non excecutable/normal data/readwrite/not accessed
    set_gate(2, 0, 0xFFFFFFFF, 0b11001111, 0b10010010);
    // user code segment granularity-G/32bit, access-present/ring3/code/excecutable/non-conforming/readable/non accessed
    set_gate(3, 0, 0xFFFFFFFF, 0b11001111, 0b11111010);
    // user data segment granularity-G/32bit, access-present/ring3/data/non excecutable/normal data/readwrite/non accesses
    set_gate(4, 0, 0xFFFFFFFF, 0b11001111, 0b11110010);
    // task state segment granularity-nonG, access-present/ring0/system/defines what system object, tss
    set_gate(5, (uint32_t)&tss_entry, sizeof(tss_entry) - 1, 0b00000000, 0b10001001);
    // passed to asm for lgdt
    struct gdt_ptr gdt_ptr;

    // init gdt_ptr
    gdt_ptr.limit = sizeof(gdt) - 1; // -1 from size of gdt for limit in pointer
    gdt_ptr.base = (uint32_t)&gdt;

    load_gdt((uint32_t)&gdt_ptr);
}