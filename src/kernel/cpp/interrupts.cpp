#include "interrupts.h"
#include "panic.h"

extern "C" void interrupt_handler(uint32_t number) {
    switch (number) {
        case 0:
            panic("Divide Error\n", 0);

        default:
            panic("Unhandled or Reserved Exception Induced\n", 1000);
    }
}