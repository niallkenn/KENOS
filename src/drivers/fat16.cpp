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

    buffer[0] = 0xF8;
    buffer[1] = 0xFF;

    buffer[2] = 0xFF;
    buffer[3] = 0xFF;

    for (int i = 4; i < 512; i++) {
        buffer[i] = 0;
    }

    if (!IDE::writeSector(4, buffer)) return false;
    if (!IDE::writeSector(132, buffer)) return false;

    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;

    for (int i = 5; i <= 131; i++) {
        if (!IDE::writeSector(i, buffer)) return false;
        if (!IDE::writeSector(i + 128, buffer)) return false;
    }

    for (int i = 260; i <= 291; i++) {
        if (!IDE::writeSector(i, buffer)) return false;
    }

    uint8_t sample[512] = {};
    const char* name = "TEST    TXT";

    for (int i = 0; i < 11; i++) {
        sample[i] = name[i];
    }

    if (!IDE::writeSector(261, sample)) return false;

    for (int i = 32; i < 32 + 11; i++) {
        sample[i] = name[i];
    }

    if (!IDE::writeSector(260, sample)) return false;

    return true;
}

kVector<DirectoryEntryName> FAT16::listRootDirectory() {
    uint8_t buffer[512];
    kVector<DirectoryEntryName> list;

    for (uint32_t sector = 260; sector <= 291; sector++) {
        if (!IDE::readSector(sector, buffer)) return {};

        DirectoryEntry* entries = reinterpret_cast<DirectoryEntry*>(buffer);

        for (int i = 0; i < 16; i++) {
            if (entries[i].name[0] == 0x00) continue;

            
            DirectoryEntryName name{};

            for (int j = 0; j < 11; j++) {
                name.name[j] = entries[i].name[j];
            }

            name.name[11] = '\0';

            list.push_back(name);
        }
    }

    return list;
}

DirectoryEntryLocation FAT16::findFreeDirectoryEntry() {
    uint8_t buffer[512];

    for (uint32_t sector = 260; sector <= 291; sector++) {
        if (!IDE::readSector(sector, buffer)) return {};

        DirectoryEntry* entries = reinterpret_cast<DirectoryEntry*>(buffer);

        for (unsigned int i = 0; i < 16; i++) {
            if (entries[i].name[0] == 0x00) return {sector, i * 32};
        }
    }

    return {};
}