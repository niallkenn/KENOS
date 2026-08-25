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

bool IDE::readSector(uint32_t lba, uint8_t* buffer) {
    uint8_t status;

    do
    {
        status = PortIO::inb(0x1F7);
    } while ((status & (1 << 7)) || !(status & (1 << 6)));

    PortIO::outb(0x1F2, 1);

    PortIO::outb(0x1F3, (char)(lba & 0xFF));
    PortIO::outb(0x1F4, (char)((lba >> 8) & 0xFF));
    PortIO::outb(0x1F5, (char)((lba >> 16) & 0xFF));
    PortIO::outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));

    PortIO::outb(0x1F7, 0x20);

    if (!waitForData()) return false;

    for (int i = 0; i < 256; i++) {
        uint16_t word = PortIO::inw(0x1F0);

        buffer[i * 2] = word & 0xFF;
        buffer[i * 2 + 1] = word >> 8;
    }

    return true;
}

bool IDE::writeSector(uint32_t lba, const uint8_t* buffer) {
    PortIO::outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));

    PortIO::outb(0x1F2, 1);

    PortIO::outb(0x1F3, lba & 0xFF);
    PortIO::outb(0x1F4, (lba >> 8) & 0xFF);
    PortIO::outb(0x1F5, (lba >> 16) & 0xFF);

    PortIO::outb(0x1F7, 0x30);

    if (!waitForData()) return false;

    for (int i = 0; i < 256; i++) {
        uint16_t word = buffer[i * 2] | (static_cast<uint16_t>(buffer[i * 2 + 1]) << 8);

        PortIO::outw(0x1F0, word);
    }

    uint8_t status;

    for (uint32_t timeout = 0; timeout < 1000000; timeout++)
    {
        status = PortIO::inb(0x1F7);

        if (!(status & (1 << 7)))
            break;

        if (timeout == 999999)
            return false;
    }

    if (status & (1 << 0))
        return false;

    return true;
}