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

    const char* fat16_str = "FAT16   ";
    int index = 0;
    for (int i = 0; i < 8; i++) {
        buffer[i + 0x36] = fat16_str[index];
        index++;
    }

    if (!IDE::writeSector(0, buffer)) return false;

    return true;
}