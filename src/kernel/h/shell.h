#pragma once
#include "definitions.h"
#include "screen.h"
#include "terminal.h"

class Shell {
    private:
        Terminal& terminal;   
    public:
        Shell(Terminal& terminal);

        void print_char(char c);
        void print(const char* string);

        void handle_enter();
        void handle_backspace();
};
