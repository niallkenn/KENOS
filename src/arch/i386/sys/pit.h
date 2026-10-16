#ifndef PIT_H
#define PIT_H

// global pit tps macro
#define PIT_TICKS_PER_SECOND 1000
#define PIT_TICKS_PER_MS (PIT_TICKS_PER_SECOND / 1000)

#include "user/definitions.h"

#define PIT_BASE_FREQUENCY 1193182

// initialise programmable interval timer to tick freqency
void init_pit(uint32_t frequency);

// hardware sleep ticks
void pit_wait_ms(uint32_t ms);

#endif