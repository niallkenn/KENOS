#include "fat16.h"
#include "ide.h"
#include "kstring.h"

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

    return true;
}

kVector<DirectoryEntryName> FAT16::listRootDirectory() {
    uint8_t buffer[512];
    kVector<DirectoryEntryName> list;

    for (uint32_t sector = 260; sector <= 291; sector++) {
        if (!IDE::readSector(sector, buffer)) return {};

        DirectoryEntry* entries = reinterpret_cast<DirectoryEntry*>(buffer);

        for (int i = 0; i < 16; i++) {
            if (entries[i].name[0] == 0) continue;
            if (entries[i].name[0] == 0xE5) continue;

            
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

int32_t FAT16::findFreeCluster() {
    uint8_t buffer[512];

    for (uint32_t sector = 4; sector < 128 + 4; sector++) {
        if (!IDE::readSector(sector, buffer)) return -1;

        for (int i = 0; i < 256; i++) {
            uint32_t cluster = (sector - 4) * 256 + i;
            if (cluster < 2) continue;

            if (buffer[2 * i + 1] == 0 && buffer[2 * i] == 0) return cluster;
        }
    }

    return -1;
}

bool FAT16::allocateCluster(uint32_t cluster) {
    uint32_t sector = 4 + cluster / 256;

    uint8_t buffer[512];

    if (!IDE::readSector(sector, buffer)) return false;

    buffer[2 * cluster] = 0xFF;
    buffer[2 * cluster + 1] = 0xFF;

    if (!IDE::writeSector(sector, buffer)) return false;

    sector += 128;

    if (!IDE::readSector(sector, buffer)) return false;

    buffer[2 * (cluster % 256)] = 0xFF;
    buffer[2 * (cluster % 256) + 1] = 0xFF;

    if (!IDE::writeSector(sector, buffer)) return false;

    return true;
}

bool is_upper(char c) {
    if (c >= 65 && c <= 90) return true;
    return false;
}

bool invalid_char(char c) {
    if (c >= 'a' && c <= 'z') return false;
    if (c >= 'A' && c <= 'Z') return false;
    if (c >= '0' && c <= '9') return false;

    return true;
}

bool FAT16::createFile(const char* filename) {
    kString input(filename);
    size_t input_size = input.size();

    if (input_size == 0) return false;

    int points = 0;

    for (size_t i = 0; i < input_size; i++) if (input[i] == '.') points++;

    if (points > 1) return false;

    char fatname[11];

    for (int i = 0; i < 11; i++) fatname[i] = ' ';

    if (points == 1) {
        size_t pointindex = 0;
        for (size_t i = 0; i < input_size; i++) {
            if (input[i] == '.') {
                pointindex = i;
                continue;
            }
            if (invalid_char(input[i])) return false;
        }

        if (pointindex == input_size - 1) return false;
        if (pointindex > 8) return false;
        if (input_size - pointindex - 1 > 3) return false;

        char prename[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

        for (size_t i = 0; i < pointindex; i++) {
            char c = input[i];
            
            if (c >= 'a' && c <='z') {
                c = c - 'a' + 'A';
            }

            prename[i] = c;
        }

        char extension[3] = {' ', ' ', ' '};

        for (size_t i = pointindex + 1; i < input_size; i++) {
            char c = input[i];
            
            if (c >= 'a' && c <='z') {
                c = c - 'a' + 'A';
            }

            extension[i - pointindex - 1] = c;
        }

        for (int i = 0; i < 8; i++) {
            fatname[i] = prename[i];
        }

        for (int i = 0; i < 3; i++) {
            fatname[i + 8] = extension[i];
        }
    } else {
        if (input_size > 8) return false;

        for (size_t i = 0; i < input_size; i++) {
            if (invalid_char(input[i])) return false;
            if (input[i] >= 'a' && input[i] <= 'z') {
                fatname[i] = input[i] - 'a' + 'A';
            } else {
                fatname[i] = input[i];
            }
        }

        if (input_size != 11) {
            for (int i = input_size; i < 11; i++) {
                fatname[i] = ' ';
            }
        }
    }

    if (fileExists(fatname)) return false;

    DirectoryEntryLocation location = findFreeDirectoryEntry();
    int32_t cluster = findFreeCluster();
    if (cluster == -1) return false;

    if (!allocateCluster(cluster)) return false;

    DirectoryEntry entry{};

    for (int i = 0; i < 11; i++) {
        entry.name[i] = fatname[i];
    }

    entry.firstClusterHigh = 0;
    entry.firstClusterLow = static_cast<uint16_t>(cluster);
    entry.fileSize = 0;
    entry.attributes = 0;

    uint8_t buffer[512];

    if (!IDE::readSector(location.sector, buffer)) return false;

    for (uint32_t i = 0; i < 32; i++) {
        buffer[location.offset + i] = reinterpret_cast<uint8_t*>(&entry)[i];
    }

    if (!IDE::writeSector(location.sector, buffer)) return false;

    return true;
}

bool FAT16::fileExists(const char* fatname) {
    kString name(fatname);

    kVector<DirectoryEntryName> names = listRootDirectory();

    for (size_t i = 0; i < names.size(); i++) {
        for (size_t j = 0; j < 11; j++) {
            if (names[i].name[j] != fatname[j]) {
                break;
            }
        }

        return true;
    }

    return false;
}

bool FAT16::findFile(const char* fatname, DirectoryEntry& entry, DirectoryEntryLocation& location) {
    uint8_t buffer[512];

    for (uint32_t sector = 260; sector <= 291; sector++) {
        if (!IDE::readSector(sector, buffer)) return false;

        DirectoryEntry* entries = reinterpret_cast<DirectoryEntry*>(buffer);
        for (int i = 0; i < 16; i++) {
            if (entries[i].name[0] == 0x00) continue;
            if (entries[i].name[0] == 0xE5) continue;

            bool match = true;

            for (int j = 0; j < 11; j++) {
                if (entries[i].name[j] != fatname[j]) {
                    match = false;
                    break;
                }
            }

            if (!match) continue;

            location.sector = sector;
            location.offset = i * 32;

            entry = entries[i];

            return true;
        }
    }

    return false;
}


bool FAT16::convertFatName(const char* filename, char fatname[11]) {
    kString input(filename);
    size_t input_size = input.size();

    if (input_size == 0) return false;

    int points = 0;

    for (size_t i = 0; i < input_size; i++) if (input[i] == '.') points++;

    if (points > 1) return false;

    for (int i = 0; i < 11; i++) fatname[i] = ' ';

    if (points == 1) {
        size_t pointindex = 0;
        for (size_t i = 0; i < input_size; i++) {
            if (input[i] == '.') {
                pointindex = i;
                continue;
            }
            if (invalid_char(input[i])) return false;
        }

        if (pointindex == input_size - 1) return false;
        if (pointindex > 8) return false;
        if (input_size - pointindex - 1 > 3) return false;

        char prename[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

        for (size_t i = 0; i < pointindex; i++) {
            char c = input[i];
            
            if (c >= 'a' && c <='z') {
                c = c - 'a' + 'A';
            }

            prename[i] = c;
        }

        char extension[3] = {' ', ' ', ' '};

        for (size_t i = pointindex + 1; i < input_size; i++) {
            char c = input[i];
            
            if (c >= 'a' && c <='z') {
                c = c - 'a' + 'A';
            }

            extension[i - pointindex - 1] = c;
        }

        for (int i = 0; i < 8; i++) {
            fatname[i] = prename[i];
        }

        for (int i = 0; i < 3; i++) {
            fatname[i + 8] = extension[i];
        }
    } else {
        if (input_size > 8) return false;

        for (size_t i = 0; i < input_size; i++) {
            if (invalid_char(input[i])) return false;
            if (input[i] >= 'a' && input[i] <= 'z') {
                fatname[i] = input[i] - 'a' + 'A';
            } else {
                fatname[i] = input[i];
            }
        }

        if (input_size != 11) {
            for (int i = input_size; i < 11; i++) {
                fatname[i] = ' ';
            }
        }
    }

    return true;
}

bool FAT16::writeFile(const char* filename, const uint8_t* data, uint32_t size) {
    
    return true;
}