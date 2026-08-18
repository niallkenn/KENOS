#include "shell.h"
#include "terminal.h"

void Shell::handle_backspace() {
    // if length of command buffer is zero, return
    terminal.backspace();
}   

void Shell::handle_enter() {
    terminal.new_line();
    terminal.put_char('>');
}

Shell::Shell(Terminal& terminal) : terminal(terminal){
    terminal.put_char('>');
}

void Shell::print_char(char c) {
    terminal.put_char(c);
}

void Shell::print(const char* string) {
    terminal.write(string);
}