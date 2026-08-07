#pragma once

#include "definitions.h"

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

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