#pragma once
#include "definitions.h"
#include "screen.h"
#include "terminal.h"

class Shell {
    private:
        Terminal& terminal;   
    public:
        Shell(Terminal& terminal);

        void handle_enter();
        void handle_backspace();
};
