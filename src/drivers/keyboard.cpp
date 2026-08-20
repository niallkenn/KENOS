#include "keyboard.h"
#include "portio.h"
const char Keyboard::scancode_map[128] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 - 0x0E
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     // 0x0F - 0x1C
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 0x1D - 0x28
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0,          // 0x29 - 0x35
    '*',    0, ' '
};

const char Keyboard::scancode_map_upper[128] = {
    0,      // 0x00
    0,      // 0x01 ESC
    '!',    // 0x02 1
    '"',    // 0x03 2
    0,    // 0x04 3
    '$',    // 0x05 4
    '%',    // 0x06 5
    '^',    // 0x07 6
    '&',    // 0x08 7
    '*',    // 0x09 8
    '(',    // 0x0A 9
    ')',    // 0x0B 0
    '_',    // 0x0C -
    '+',    // 0x0D =
    0,      // 0x0E backspace
    0,      // 0x0F tab

    'Q',    // 0x10
    'W',    // 0x11
    'E',    // 0x12
    'R',    // 0x13
    'T',    // 0x14
    'Y',    // 0x15
    'U',    // 0x16
    'I',    // 0x17
    'O',    // 0x18
    'P',    // 0x19
    '{',    // 0x1A [
    '}',    // 0x1B ]
    0,      // 0x1C enter
    0,      // 0x1D ctrl

    'A',    // 0x1E
    'S',    // 0x1F
    'D',    // 0x20
    'F',    // 0x21
    'G',    // 0x22
    'H',    // 0x23
    'J',    // 0x24
    'K',    // 0x25
    'L',    // 0x26
    ':',    // 0x27 ;
    '@',    // 0x28 '
    '~',    // 0x29 `

    0,      // 0x2A left shift
    '|',    // 0x2B backslash
    'Z',    // 0x2C
    'X',    // 0x2D
    'C',    // 0x2E
    'V',    // 0x2F
    'B',    // 0x30
    'N',    // 0x31
    'M',    // 0x32
    '<',    // 0x33 ,
    '>',    // 0x34 .
    '?',    // 0x35 /
    0,      // 0x36 right shift
    0,      // 0x37 keypad *
    0,      // 0x38 alt
    ' ',    // 0x39 space

    0,      // 0x3A caps lock
    0,      // 0x3B F1
    0,      // 0x3C F2
    0,      // 0x3D F3
    0,      // 0x3E F4
    0,      // 0x3F F5
    0,      // 0x40 F6
    0,      // 0x41 F7
    0,      // 0x42 F8
    0,      // 0x43 F9
    0,      // 0x44 F10
    0,      // 0x45 Num Lock
    0,      // 0x46 Scroll Lock
    0,      // 0x47 Home
    0,      // 0x48 Up
    0,      // 0x49 Page Up
    0,      // 0x4A keypad -
    0,      // 0x4B Left
    0,      // 0x4C keypad 5
    0,      // 0x4D Right
    0,      // 0x4E keypad +
    0,      // 0x4F End
    0,      // 0x50 Down
    0,      // 0x51 Page Down
    0,      // 0x52 Insert
    0,      // 0x53 Delete

    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0
};

bool Keyboard::shift_pressed = false;
bool Keyboard::caps_lock = false;

uint8_t Keyboard::last_scancode = 0;

Shell* Keyboard::active_shell = nullptr;

char Keyboard::get_char() {
    uint8_t scancode = PortIO::inb(0x60);

    if (scancode == last_scancode) return 0;

    last_scancode = scancode;

    bool released = scancode & 0x80;
    uint8_t key = scancode & 0x7F;

    if (key == 0x2A || key == 0x36) {
        shift_pressed = !released;
        return 0;
    }

    if (key == 0x3A && !released) {
        caps_lock = !caps_lock;
        return 0;
    }

    if (released) return 0;

    if (key >= 128) return 0;

    char lower = scancode_map[key];
    char upper = scancode_map_upper[key];

    if (lower >= 'a' && lower <= 'z') {
        if (shift_pressed ^ caps_lock) return upper;

        return lower;
    }

    if (shift_pressed) return upper;

    return lower;
}

void Keyboard::set_shell(Shell* shell) {
    active_shell = shell;
}