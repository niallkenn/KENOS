#ifndef PIT_H
#define PIT_H

#include "user/definitions.h"

#define PIT_BASE_FREQUENCY 1193182

// initialise programmable interval timer to tick freqency
void init_pit(uint32_t frequency);

// hardware sleep ticks
void pit_wait(uint64_t ticks);

#endif