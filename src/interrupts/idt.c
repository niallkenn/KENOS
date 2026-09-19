#include "idt.h"

// init
idt_entry_t idt[256] = {};
idtr_t idtr;

// set gate function definition
static void set_gate(int index, uint32_t address, uint16_t selector, uint8_t flags) {
    idt[index].flags = flags;
    idt[index].selector = selector;
    idt[index].zero = 0;

    idt[index].offset_low = address & 0xFFFF;
    idt[index].offset_high = (address >> 16) & 0xFFFF;
}

// load idt function
void load_idt(idtr_t* idtr) {
    asm volatile(
        "lidt %0"
        :
        : "m" (*idtr)
    );
}

// initialse idt
void init_idt(){
    // assign idtr values
    idtr.base = (uint32_t)idt;
    idtr.limit = sizeof(idt) - 1;
    
    // set gates for isr stubs
    set_gate(0,  (uint32_t)isr0,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(1,  (uint32_t)isr1,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(2,  (uint32_t)isr2,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(3,  (uint32_t)isr3,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(4,  (uint32_t)isr4,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(5,  (uint32_t)isr5,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(6,  (uint32_t)isr6,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(7,  (uint32_t)isr7,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(8,  (uint32_t)isr8,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(9,  (uint32_t)isr9,  KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(10, (uint32_t)isr10, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(11, (uint32_t)isr11, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(12, (uint32_t)isr12, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(13, (uint32_t)isr13, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(14, (uint32_t)isr14, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(15, (uint32_t)isr15, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(16, (uint32_t)isr16, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(17, (uint32_t)isr17, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(18, (uint32_t)isr18, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(19, (uint32_t)isr19, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(20, (uint32_t)isr20, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(21, (uint32_t)isr21, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(22, (uint32_t)isr22, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(23, (uint32_t)isr23, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(24, (uint32_t)isr24, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(25, (uint32_t)isr25, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(26, (uint32_t)isr26, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(27, (uint32_t)isr27, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(28, (uint32_t)isr28, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(29, (uint32_t)isr29, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(30, (uint32_t)isr30, KERNEL_CODE_SELECTOR, 0x8E);
    set_gate(31, (uint32_t)isr31, KERNEL_CODE_SELECTOR, 0x8E);

    // set gate for syscall isr 0x80, 0xEE -> 0x8E | ring 3
    set_gate(0x80, (uint32_t)isr128, KERNEL_CODE_SELECTOR, 0xEE);

    // load the idtr
    load_idt(&idtr);
}