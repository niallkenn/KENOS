#include "interrupts.h"
#include "panic.h"

extern "C" void interrupt_handler(Registers* regs) {
    switch (regs->interrupt_number) {
        case 0:
            panic("Divide Error\n", regs);
            break;
        case 14:
            panic("Page fault", regs);
            break;

        default:
            panic("Unhandled or Reserved Exception Induced\n", regs);
            break;
    }
}