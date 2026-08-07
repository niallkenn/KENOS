#include "panic.h"
#include "screen.h"
#include "terminal.h"


void panic(const char* message, Registers* regs) {
    asm volatile("cli");

    uint8_t color = 40;
    Terminal t(5, 5, 315, 195, WHITE, color);
    Screen::clear_screen(color);

    t.write("              KERNEL PANIC\n\n\n");

    uint32_t number = regs->interrupt_number;

    if (number == 1000) {
        t.write(message);
    } else if (number < 32) {
        t.write("Exception:         ");
        t.write(message);
        t.write("\n\nInterrupt:         ");
        t.write_uint(number);
        t.write("\n\nEIP:               ");
        t.write_hex(regs->eip);
        t.write("\nError Code:        ");
        t.write_hex(regs->error_code);
    }
    

    t.write("\n\n\n\nSystem halted.");

    while (true) asm volatile("hlt");
}