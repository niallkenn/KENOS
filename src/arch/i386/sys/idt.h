#ifndef IDT_H
#define IDT_H

#include "user/definitions.h"
#include "interrupts.h"

// interrupt descriptor table entry struct, holds isr address, code segment selector, flags.
struct __attribute__((packed)) idt_entry {
    uint16_t isr_address_low; // low bytes of isr addr
    uint16_t code_segment_selector; // usually kernel code segment selector
    uint8_t reserved; // 0
    uint8_t flags; // present, dpl, 4 bits for type, decides behavior of the gate
    uint16_t isr_address_high;
};

// interrupt descriptor table pointer structure, passed to asm to load idt, lidt
struct __attribute__((packed)) idt_ptr {
    uint16_t limit; // size of idt in bytes minus 1
    uint32_t base; // physical memory address of the idt
};

// global idt array
extern struct idt_entry idt[256];
// global idt ptr to pass to asm
extern struct idt_ptr idt_ptr;

// initialise interrupt descriptor table, load into idtr
void init_idt(void);

// common interrupt handler c function for isr stubs, takes interrupt frame argument
struct interrupt_frame* interrupt_handler(struct interrupt_frame* frame);

// asm wrapper for loading idt pointer
extern void load_idt(uint32_t idt_ptr);

// isr stubs, defined in idt.S
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

// irqs
extern void isr32(void); // irq 0, timer

#endif