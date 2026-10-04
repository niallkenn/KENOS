#ifndef PITH
#define PITH

#include "definitions.h"
#include "interrupts.h"

extern volatile uint32_t timer_ticks;

void init_pit(uint32_t freqency_hz);
registers_t* irq0_handler(registers_t* registers);

#endif