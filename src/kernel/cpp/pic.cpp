#include "pic.h"

constexpr uint16_t PIC1_COMMAND = 0x20;
constexpr uint16_t PIC1_DATA    = 0x21;
constexpr uint16_t PIC2_COMMAND = 0xA0;
constexpr uint16_t PIC2_DATA    = 0xA1;

inline uint8_t inb(uint16_t port) {
    return PortIO::inb(port);
}

inline void outb(uint16_t port, uint8_t value) {
    PortIO::outb(port, value);
}

void io_wait() {
    outb(0x80, 0);
}

void Pic::initialize() {
    uint8_t master_mask = inb(PIC1_DATA);
    uint8_t slave_mask = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11);
    io_wait();
    outb(PIC2_COMMAND, 0x11);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    outb(PIC1_DATA, master_mask);
    io_wait();
    outb(PIC2_DATA, slave_mask);
    io_wait();
}