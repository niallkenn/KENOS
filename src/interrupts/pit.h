#ifndef PITH
#define PITH

#include "definitions.h"

extern volatile uint32_t timer_ticks;

void init_pit(uint32_t freqency_hz);

#endif