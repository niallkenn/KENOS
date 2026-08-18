#include "pit.h"
#include "portio.h"

volatile uint32_t timer_ticks = 0;

void Pit::initialize(uint32_t frequency) {
    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    PortIO::outb(PIT_COMMAND, 0x34);

    PortIO::outb(PIT_CHANNEL_0, divisor & 0xFF);

    PortIO::outb(PIT_CHANNEL_0, (divisor >> 8) & 0xFF);
}