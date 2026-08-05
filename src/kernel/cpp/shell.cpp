#include "shell.h"
#include "terminal.h"

void Shell::handle_backspace() {
    // if length of command buffer is zero, return
    terminal.backspace();
}   

void Shell::handle_enter() {
    terminal.new_line();
    terminal.put_char('>', terminal.get_fg());
    last_arrow_y = terminal.get_cy();
}

Shell::Shell(Terminal& terminal) : terminal(terminal){
    last_arrow_y = terminal.get_cy();
}