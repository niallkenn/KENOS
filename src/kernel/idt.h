#pragma once

#include "definitions.h"

class Idt {
    private:
        struct __attribute((packed)) IDTEntry {
            uint16_t offset_low;
            uint16_t selector;
            uint8_t zero;
            uint8_t flags;
            uint16_t offset_high;
        };

        static IDTEntry idt[256];
    public:
        static void initialize();
};