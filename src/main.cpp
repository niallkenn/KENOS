#include "definitions.h"
#include "keyboard.h"
#include "shell.h"
#include "idt.h"
#include "terminal.h"
#include "pic.h"
#include "interrupts.h"
#include "pit.h"
#include "memorymap.h"
#include "frameallocator.h"
#include "paging.h"
#include "heapallocator.h"
#include "kstring.h"
#include "kvector.h"
#include "ide.h"
#include "panic.h"

extern "C" void main() {
    Idt::initialize();

    Pic::initialize();

    uint32_t pit_frequency = 100;
    Pit::initialize(pit_frequency);

    MemoryMap::initialize();

    FrameAllocator::initialise();

    Paging::initialise();

    HeapAllocator::initialise();

    if (!IDE::initialise()) panic("Could not read disk");
    
    Terminal terminal(5, 5, 315, 195, LIGHT_GREY, BLACK);
    Shell shell(terminal);
    
    Keyboard::set_shell(&shell);

    enable_interrupts();

    while (1) {
        asm volatile("hlt");
    }
}
