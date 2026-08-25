#include "ide.h"
#include "portio.h"

uint16_t IDE::identifyReturn[256] = {};
bool IDE::driveFound = false;

bool IDE::waitForData() {
    for (uint32_t timeout = 0; timeout < 1000000; timeout++) {
        uint8_t status = PortIO::inb(0x1F7);

        if (status & (1 << 0)) return false;

        if (status & (1 << 3)) return true;
    }

    return false;
}

bool IDE::initialise() {
    PortIO::outb(0x1F6, 0xA0);

    for (int i = 0; i < 4; i++) {
        PortIO::inb(0x1F7);
    }

    PortIO::outb(0x1F2, 0);
    PortIO::outb(0x1F3, 0);
    PortIO::outb(0x1F4, 0);
    PortIO::outb(0x1F5, 0);

    PortIO::outb(0x1F7, 0xEC);

    uint8_t status = PortIO::inb(0x1F7);

    if (status == 0) return false;

    if (!waitForData()) return false;

    for (int i = 0; i < 256; i++) {
        identifyReturn[i] = PortIO::inw(0x1F0);
    }

    driveFound = true;
    return true;
}