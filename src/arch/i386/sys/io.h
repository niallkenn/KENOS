#ifndef IO_H
#define IO_H

#include "user/definitions.h"

// read in 1 byte from hardware port. asm wrapper
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;

    // inline asm, inb
    asm volatile(
        "inb %w1, %b0"
        : "=a" (ret) // outputs
        : "Nd" (port) // inputs
        : "memory" // memory changes
    );

    return ret;
}

static inline void outb(uint16_t port, uint8_t value) {
    // inline asm, outb
    asm volatile(
        "outb %b0, %w1"
        : // no outputs
        : "a" (value), "Nd" (port) // inputs
        : "memory" // memory changed  
    );
}

#endif