#include "panic.h"
#include "screen.h"

void panic(const char* message, unsigned int number) {
    asm volatile("cli");

    Screen::clear_screen(DARK_RED);
    

    while (true) asm volatile("hlt");
}