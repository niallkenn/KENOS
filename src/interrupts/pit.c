#include "pit.h"
#include "portio.h"

volatile uint32_t timer_ticks = 0;

void init_pit(uint32_t frequency_hz) {
    uint32_t divisor = 1193182 / frequency_hz;

    outb(0x43, 0x36);

    outb(0x40, (uint8_t)divisor & 0xFF);
    outb(0x40, (uint8_t)(divisor >> 8) & 0xFF);
}