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

    
    char* a = new char[10];
    char* b = new char[20];

    if (a == b)
        shell.print("FAIL\n", fg_color);
    else
        shell.print("PASS\n", fg_color);
        
    while (true) {
        char c = Keyboard::get_char();

        if (c != 0) {
            shell.print_char(c, fg_color);
        }
    }
}
