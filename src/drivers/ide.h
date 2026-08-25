#pragma once

#include "definitions.h"

class IDE {
    private:
        static bool waitForData();

    public:
        IDE() = delete;

        static bool initialise();
        static bool readSector(uint32_t lba, uint8_t* buffer);
        static bool writeSector(uint32_t lba, const uint8_t* buffer);
        
        static uint16_t identifyReturn[256];
        static bool driveFound;
};