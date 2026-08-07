#include "keyboard.h"
#include "portio.h"
const char Keyboard::scancode_map[128] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 - 0x0E
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     // 0x0F - 0x1C
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 0x1D - 0x28
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0,          // 0x29 - 0x35
    '*',    0, ' '
};

uint8_t Keyboard::last_scancode = 0;

Shell* Keyboard::active_shell = nullptr;

char Keyboard::get_char() {
    uint8_t scancode = PortIO::inb(0x60);

    if (scancode == last_scancode) return 0;
    last_scancode = scancode;

    if (scancode & 0x80) return 0;

    if (scancode < 128) {
        return scancode_map[scancode];
    } else return 0;
}

void Keyboard::set_shell(Shell* shell) {
    active_shell = shell;
}