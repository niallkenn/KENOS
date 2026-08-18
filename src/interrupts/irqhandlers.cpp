#include "irqhandlers.h"
#include "pit.h"
#include "keyboard.h"
#include "pic.h"

void handle_irq0() {
    timer_ticks++;

    Pic::send_eoi(0);
}

void handle_irq1() {
    char c = Keyboard::get_char();

    if (c) {
        if (c == '\n') {
            Keyboard::active_shell->handle_enter();
        } else if (c == '\b') {
            Keyboard::active_shell->handle_backspace();
        } else {
            Keyboard::active_shell->print_char(c);
        }
    }

    Pic::send_eoi(1);
}