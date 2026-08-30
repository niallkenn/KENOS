#pragma once

#include "definitions.h"
#include "kvector.h"

struct DirectoryEntry {
    char name[11];
    uint8_t  attributes;
    uint8_t  reserved;
    uint8_t  creationTimeTenth;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessDate;
    uint16_t firstClusterHigh;
    uint16_t writeTime;
    uint16_t writeDate;
    uint16_t firstClusterLow;
    uint32_t fileSize;
} __attribute__((packed));

struct DirectoryEntryName {
    char name[12];
};

struct DirectoryEntryLocation {
    uint32_t sector;
    uint32_t offset;
};

class FAT16 {
    private:

    public:
        FAT16() = delete;

        static bool initialise();
        static bool format();
        
        static kVector<DirectoryEntryName> listRootDirectory();
        static DirectoryEntryLocation findFreeDirectoryEntry();
        static int32_t findFreeCluster();
        static bool allocateCluster(uint32_t cluster);
};