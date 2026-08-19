#include "shellcommands.h"

void k_clear(Shell& shell) {
    if (shell.command_buffer.size() > 1) {
        shell.terminal.write("\nclear: invalid options\n");
    }

    shell.terminal.clear();
}

void k_echo(Shell& shell) {
    shell.terminal.new_line();
    for (int i = 1; i < shell.command_buffer.size(); i++) {
        shell.terminal.write(shell.command_buffer[i].c_str());
        shell.terminal.put_char(' ');
    }
    shell.terminal.new_line();
}

void k_command_error(Shell& shell) {
    shell.terminal.write("\ncommand not found: ");
    shell.terminal.write(shell.command_buffer[0].c_str());
    shell.terminal.new_line();
}