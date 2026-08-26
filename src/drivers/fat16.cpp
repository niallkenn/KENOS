#include "fat16.h"
#include "ide.h"

bool FAT16::initialise() {
    uint8_t buffer[512];

    if (!IDE::readSector(0, buffer)) return false;

    if (buffer[510] != 0x55 || buffer[511] != 0xAA) return false;

    const char* fat16_str = "FAT16   ";
    int index = 0;
    for (int i = 0x36; i < 0x36 + 8; i++) {
        if (buffer[i] != fat16_str[index]) return false;
        index++;
    }

    return true;
}

bool FAT16::format() {
    uint8_t buffer[512] = {};

    buffer[510] = 0x55;
    buffer[511] = 0xAA;

    buffer[0x0B] = 0x00;
    buffer[0x0C] = 0x02;

    buffer[0x0D] = 4;

    buffer[0x0E] = 4;
    buffer[0x0F] = 0;

    buffer[0x10] = 2;

    buffer[0x11] = 0x00;
    buffer[0x12] = 0x02;

    buffer[0x13] = 0;
    buffer[0x14] = 0;

    buffer[0x15] = 0xF8;

    buffer[0x16] = 128;
    buffer[0x17] = 0;

    buffer[0x20] = 0x00;
    buffer[0x21] = 0x00;
    buffer[0x22] = 0x02;
    buffer[0x23] = 0x00;

    const char* fat16_str = "FAT16   ";
    int index = 0;
    for (int i = 0; i < 8; i++) {
        buffer[i + 0x36] = fat16_str[index];
        index++;
    }

    if (!IDE::writeSector(0, buffer)) return false;

    return true;
}