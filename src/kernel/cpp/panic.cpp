#include "panic.h"
#include "screen.h"
#include "terminal.h"

void panic(const char* message, unsigned int number) {
    asm volatile("cli");

    uint8_t color = 40;
    Terminal t(0, 0, 320, 200, WHITE, color);

    t.write("              KERNEL PANIC\n\n\n", color);

    if (number == 1000) {
        t.write(message, color);
        t.write("\n\n\n", color);
    } else if (number < 32) {
        t.write("Exception:\n\n\t", color);
        t.write(message, color);
        t.write("\n\n\n", color);
    }

    t.write("System halted.", color);

    while (true) asm volatile("hlt");
}