#include "definitions.h"
#include "keyboard.h"
#include "shell.h"
#include "idt.h"
#include "terminal.h"
#include "pic.h"
#include "interrupts.h"
#include "pit.h"
#include "memorymap.h"

extern "C" void main() {
    Idt::initialize();

    Pic::initialize();

    uint32_t pit_frequency = 100;
    Pit::initialize(pit_frequency);

    MemoryMap::initialize();
    
    Terminal t(5, 5, 315, 195, LIGHT_GREY, BLACK);
    Shell s(t);
    
    Keyboard::set_shell(&s);

    enable_interrupts();

    while (1) {
        asm volatile("hlt");
    }
}
