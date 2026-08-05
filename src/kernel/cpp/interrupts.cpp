#include "interrupts.h"
#include "panic.h"

extern "C" void interrupt_handler(uint32_t number) {
    switch (number) {
        default:
            panic("Unhandled or Reserved Exception Induced", 1000);
        case 0:
            panic("Divide Error", 0);
    }
}