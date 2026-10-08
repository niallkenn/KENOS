#ifndef TSS_H
#define TSS_H

#include "user/definitions.h"

// task state segment structure, holds cpu 
struct __attribute__((packed)) tss_entry {
    uint32_t last_tss;
    uint32_t esp0; // important, kernel stack pointer
    uint32_t ss0; // important, kernel stack segment
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
    uint16_t iomap_base; // important for user i/o permissions
};

extern struct tss_entry tss_entry;

extern void init_tss_asm(void);

void init_tss(void);

#endif