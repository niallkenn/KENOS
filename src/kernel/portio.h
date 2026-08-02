#pragma once

class PortIO {
    public:
        PortIO();

        static inline void outb(uint16_t port, uint8_t value) {
            asm volatile(
                "outb %b0, %w1"
                :
                : "a"(value), "Nd"(port)
                : "memory"
            );
        }

        static inline uint8_t inb(uint16_t port) {
            uint8_t result;

            asm volatile(
                "inb %w1, %b0"
                : "=a"(result)
                : "Nd"(port)
                : "memory"
            );
            return result;
        }
};