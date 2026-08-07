#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"
#include "memoryallocator.h"
#include "idt.h"
#include "terminal.h"
#include "pic.h"
#include "interrupts.h"

extern "C" void main() {
    Idt::initialize();
    Pic::initialize();
    enable_interrupts();

    Terminal t(5, 5, 315, 195, LIGHT_GREY, BLACK);
    Shell s(t);
}
