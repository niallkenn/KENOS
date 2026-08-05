#pragma once

#include "definitions.h"
#include "shell.h"

class Keyboard {
    private:
        static const char scancode_map[128];

        static uint8_t last_scancode;

    public:
        static char get_char();
};

void handle_enter(Shell& shell);