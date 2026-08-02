#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"

extern "C" void main() {
    Shell shell(7, 7, 313, 193, YELLOW);
    
    shell.clear_screen(YELLOW);
    shell.print_char('>', DARK_BLUE);
    
    while (1) {
        char c = Keyboard::get_char();
        if (c!=0) shell.print_char(c, DARK_BLUE);
    }
}
