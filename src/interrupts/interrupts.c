#include "interrupts.h"

// interupt handler dispatcher definition
void interrupt_handler(registers_t* registers) {
    uint32_t vector = registers->interrupt_number;
    if (vector < 32) {

    } else {

    }
    
    asm volatile("cli");
    for(;;) asm volatile("hlt");
}