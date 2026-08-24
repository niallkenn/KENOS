#pragma once

#include "definitions.h"

class IDE {
    private:

    public:
        IDE() = delete;

        static bool initialise();
        static uint16_t identifyReturn[256];
};