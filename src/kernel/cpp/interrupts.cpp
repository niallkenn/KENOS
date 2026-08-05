#include "interrupts.h"

extern "C" void interrupt_handler(uint32_t number) {
    switch (number) {
        case 0:
            asm volatile("cli; hlt");
    }
}