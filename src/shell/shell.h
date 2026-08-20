#pragma once
#include "definitions.h"
#include "screen.h"
#include "terminal.h"
#include "kvector.h"
#include "kstring.h"

class Shell {
    private:
    
    public:
        Terminal& terminal;
        kVector<kString> command_buffer;
    
        Shell(Terminal& terminal);
        
        void print_char(char c);
        void print(const char* string);

        void handle_enter();
        void handle_backspace();
};
