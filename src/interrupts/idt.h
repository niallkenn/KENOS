#include "definitions.h"

// interrupt descriptor table struct info
typedef struct idt_entry_t {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

// idtr struct definition
typedef struct idtr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

// global idt
extern idt_entry_t idt[256];
// global idtr, for loading idt
extern idtr_t idtr;

// lidt asm wrapper
void load_idt(idtr_t* idtr);
void init_idt();