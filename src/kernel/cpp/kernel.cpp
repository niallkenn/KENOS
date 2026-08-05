#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"
#include "memoryallocator.h"
#include "idt.h"
#include "terminal.h"

extern "C" void main() {
    Idt::initialize();

    Terminal t(5, 5, 315, 195, LIGHT_GREY, BLACK);
    Shell s(t);

    while (true) {
        char c = Keyboard::get_char();

        if (c == 0) continue;

        if (c == '\b') {
            s.handle_backspace();
            continue;
        }

        if (c == '\n') {
            s.handle_enter();
            continue;
        }

        if (c == '0') {
            int x = 0;
            int y = 5/x;
            x = y;
        }

        t.put_char(c, t.get_fg());
    }
}
