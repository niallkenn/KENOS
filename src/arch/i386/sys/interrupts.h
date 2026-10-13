#ifndef INTUPTS_H
#define INTUPTS_H

#include "user/definitions.h"

// interrupt frame pushed by CPU when interrupt occurs
struct __attribute__((packed)) interrupt_frame {
    // manually pushed segment registers
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    // general purpose registers, manually pushed
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // pushed manually in all isr stubs
    uint32_t vector;

    // sometimes cpu pushes depending on the interrupt
    uint32_t error_code;
    // cpu automatically pushes these when interrupt occurs
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp; // used when moving privelege levels. iret pops these 2 when switching cpl, pops segment registers. and the useresp, the stack of the user process jumping to
    uint32_t ss; // holds user data selector, as only used for switching to ring3
};

// global irq0 timer counter
extern volatile uint64_t timer_ticks;

// common exception handler, manages interrupt vectors 0 - 31
void handle_exception(uint32_t vector);
// common irq handler, irq 0 and 1, vector - 32
void handle_irq(uint32_t vector);

#endif