#pragma once

#include "definitions.h"
#include "shell.h"

class Keyboard {
    private:
        static const char scancode_map[128];

        static uint8_t last_scancode;

        
    public:
        static void set_shell(Shell* shell);
        Keyboard() = delete;
        static Shell* active_shell;
        static char get_char();
};