#pragma once

#include "definitions.h"

extern "C" void isr0();
extern "C" void isr14();

class Idt {
    private:
        Idt() = default;

        struct __attribute((packed)) IDTEntry {
            uint16_t offset_low;
            uint16_t selector;
            uint8_t zero;
            uint8_t flags;
            uint16_t offset_high;
        };

        
        static IDTEntry idt[256];
        static void set_gate(size_t index, uint32_t handler_address, uint16_t selector, uint8_t flags);
    public:
        struct __attribute((packed)) IDTR {
            uint16_t limit;
            uint32_t base;
        }; 

        static void initialize();
};