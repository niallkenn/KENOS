#include "ide.h"
#include "portio.h"

uint16_t IDE::identifyReturn[256] = {};

bool IDE::initialise() {
    PortIO::outb(0x1F6, 0xA0);

    PortIO::outb(0x1F2, 0);
    PortIO::outb(0x1F3, 0);
    PortIO::outb(0x1F4, 0);
    PortIO::outb(0x1F5, 0);

    PortIO::outb(0x1F7, 0xEC);

    uint8_t status = PortIO::inb(0x1F7);

    if (status == 0) return false;

    while (status & (1 << 7)) status = PortIO::inb(0x1F7);

    if (status & 1) return false;

    while (!(status & (1 << 3))) status = PortIO::inb(0x1F7);

    for (int i = 0; i < 256; i++) {
        identifyReturn[i] = PortIO::inw(0x1F0);
    }

    return true;
}