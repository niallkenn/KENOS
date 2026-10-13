#include "idt.h"

//.initialise global variables
struct idt_entry idt[256] = {};
struct idt_ptr idt_ptr;

// sets gate for specific index in the idt
static void set_gate(int index, uint32_t isr_address, uint16_t selector, uint8_t flags) {
    // set the isr address words, split address into 2 parts
    idt[index].isr_address_low = isr_address & 0xFFFF;
    idt[index].isr_address_high = (isr_address >> 16) & 0xFFFF;
    
    idt[index].reserved = 0;
    idt[index].flags = flags;
    idt[index].code_segment_selector = selector;
}

void init_idt(void) {
    // init fields in the idt pointer
    // base is the address of idt array
    idt_ptr.base = (uint32_t)idt;
    // limit is size of idt - 1;
    idt_ptr.limit = sizeof(idt) - 1;

    // set gates in idt
    // index, address of isr, kernel code selector 0x08, flags
    // 0x8E -> 0b10001110 -> present, ring 0, kernel mode interrupt gate type. clears if flag, used for exceptions and hardware irqs
    // 0xEE -> 0b11101110 -> present, ring 3, user mode interrupt gate tyep. same as 8E, but allows excecuting in ring 3. for standard 0x80 system calls

    set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    // irq 0, pit timer
    set_gate(32, (uint32_t)isr32, 0x08, 0x8E);
    
    // load idt with asm function. loads idtr with idt ptr
    load_idt((uint32_t)&idt_ptr);
}

struct interrupt_frame* interrupt_handler(struct interrupt_frame* frame) {
    if (frame->vector < 32) {
        handle_exception(frame->vector); // exception
    } else if (frame->vector == 32) {
        handle_irq(32); // irq 0, timer
    }

    return frame;
}