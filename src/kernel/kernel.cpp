#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"

extern "C" void main() {
    Shell shell(50, 50, 270, 150, BLACK);

    shell.print(">", LIGHT_GREY);
    
    while (1) {
        char c = Keyboard::get_char();
        if (c!=0) shell.print_char(c, LIGHT_GREY);
    }
}
