#ifndef PORTIOH
#define PORTIOH

#include "definitions.h"

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

#endif