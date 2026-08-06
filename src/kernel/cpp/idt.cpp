#include "idt.h"

extern "C" void load_idt(Idt::IDTR* idtr);

Idt::IDTEntry Idt::idt[] = {};

void Idt::set_gate(size_t index, uint32_t handler_address, uint16_t selector, uint8_t flags) {
    idt[index].flags = flags;
    idt[index].selector = selector;
    idt[index].zero = 0;

    idt[index].offset_low = (handler_address & 0xFFFF);
    idt[index].offset_high = ((handler_address >> 16) & 0xFFFF);
}

void Idt::initialize() {
    set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    set_gate(14, (uint32_t)isr14, 0x08, 0x8E);

    IDTR idtr;
    idtr.limit = sizeof(idt) - 1;
    idtr.base = reinterpret_cast<uint32_t>(idt);

    load_idt(&idtr);
}