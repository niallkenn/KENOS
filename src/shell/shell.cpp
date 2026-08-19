#include "shell.h"
#include "terminal.h"
#include "shellcommands.h"

void Shell::handle_backspace() {
    if (command_buffer[0].isEmpty()) return;

    if (command_buffer[command_buffer.size() - 1].isEmpty()) {
        if (command_buffer.size() > 1) {
            command_buffer.pop_back();
            terminal.backspace();
        }
        return;
    } 

    command_buffer[command_buffer.size() - 1].pop_back();
    
    terminal.backspace();
}   

void Shell::handle_enter() {
    if (command_buffer[0].isEmpty()) terminal.new_line();
    else if (command_buffer[0].equalTo("clear")) k_clear(*this);
    else if (command_buffer[0].equalTo("echo")) k_echo(*this);
    else k_command_error(*this);

    command_buffer.clear();
    command_buffer.push_back(kString());
    terminal.write("KENOS>");
}

Shell::Shell(Terminal& terminal) : terminal(terminal), command_buffer() {
    command_buffer.push_back(kString());
    terminal.write("KENOS>");
}

void Shell::print_char(char c) {
    if (c == ' ') {
        if (!command_buffer[command_buffer.size() - 1].isEmpty())
            command_buffer.push_back(kString());

        terminal.put_char(c);
        return;

    }

    command_buffer[command_buffer.size() - 1].push_back(c);

    terminal.put_char(c);
}

void Shell::print(const char* string) {
    terminal.write(string);
}