#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"
#include "memoryallocator.h"

extern "C" void main() {
    uint8_t fg_color = LIGHT_GREY;
    uint8_t bg_color = BLACK;
    Shell shell = init_shell(7, 7, 313, 193, fg_color, bg_color);

    char* s1 = new char[20]{"Hey there"};
    char* s2 = new char[20]{" sigma.\n"};

    shell.print(s1, fg_color);
    shell.print(s2, DARK_RED);

    delete[] s2;

    char* s3 = new char[20]{" not sigma."};

    shell.print(s1, fg_color);
    shell.print(s2, PINK);

    while (true) {
        char c = Keyboard::get_char();

        if (c != 0) {
            shell.print_char(c, fg_color);
        }
    }
}
