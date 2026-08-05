#include "panic.h"

[[noreturn]] void panic(const char* message) {
    Shell shell(5, 5, 320, 200, 40);
    shell.clear_screen(40);

    shell.print("KERNEL PANIC\n", WHITE);
    shell.print(message, WHITE);

    asm volatile("cli");
    while (true) asm volatile("hlt");
}