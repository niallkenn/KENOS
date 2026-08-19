#include "shellcommands.h"

void k_clear(Shell& shell) {
    shell.terminal.clear();
}

void k_command_error(const char* str, Shell& shell) {
    shell.terminal.write("\ncommand not found: ");
    shell.terminal.write(str);
    shell.terminal.new_line();
}