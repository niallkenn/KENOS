#pragma once
#include "definitions.h"
class Keyboard {
    private:
        static const char scancode_map[128];

        static uint8_t last_scancode;

    public:
        static char get_char();
};