#ifndef IDTH
#define IDTH

#include "definitions.h"

// selector definitions
#define KERNEL_CODE_SELECTOR 0x08

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

// asm isr stubs
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// syscall isr
extern void isr128();

// lidt asm wrapper
void load_idt(idtr_t* idtr);
// init interrupt descriptor table
void init_idt();

#endif