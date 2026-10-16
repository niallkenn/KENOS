#include "pit.h"
#include "interrupts.h"
#include "io.h"

void init_pit(uint32_t frequency) {
    // interrupt ticks, timer ticks per second.
    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    // tell pit to use square wave mode, and expect incoming 16 bit divisor
    outb(0x43, 0x36);

    // send pit low byte of divisor
    outb(0x40, (uint8_t)divisor & 0xFF);
    // high byte of divisor
    outb(0x40, (uint8_t)(divisor >> 8) & 0xFF);
}

void pit_wait_ms(uint32_t ms) {
    // ticks to wait for
    uint64_t ticks = ms * PIT_TICKS_PER_MS;
    // wait until target
    uint64_t target = timer_ticks + ticks;

    while (timer_ticks < target) asm volatile ("hlt");
}