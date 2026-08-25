#pragma once

#include "definitions.h"

class IDE {
    private:
        static bool waitForData();

    public:
        IDE() = delete;

        static bool initialise();
        static uint16_t identifyReturn[256];
        static bool driveFound;
};