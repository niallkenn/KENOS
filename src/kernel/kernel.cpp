#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"
#include "vector.h"
#include "string.h"

extern "C" void main() {
    uint8_t fg_color = LIGHT_GREY;
    uint8_t bg_color = BLACK;
    Shell shell = init_shell(7, 7, 313, 193, fg_color, bg_color);

    while (true) {
        char c = Keyboard::get_char();

        if (c != 0) {
            shell.print_char(c, fg_color);
        }
    }
}
