#include "keyboard.h"
#include "portio.h"
#include "pic.h"

bool keyboard_shift_pressed = false;
bool keyboard_caps_lock = false;
uint8_t keyboard_last_scancode = 0;

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile uint32_t keyboard_head = 0, keyboard_tail = 0;

char keyboard_get_char(void) {
    uint8_t scancode = inb(0x60);

    if (scancode == keyboard_last_scancode) return 0;

    keyboard_last_scancode = scancode;

    bool released = scancode & 0x80;
    uint8_t key = scancode & 0x7F;

    if (key == 0x2A || key == 0x36) {
        keyboard_shift_pressed = !released;
        return 0;
    }

    if (key == 0x3A && !released) {
        keyboard_caps_lock = !keyboard_caps_lock;
        return 0;
    }

    if (released) return 0;

    if (key >= 128) return 0;

    char lower = keyboard_scancode_map_lower[key];
    char upper = keyboard_scancode_map_upper[key];

    if (lower >= 'a' && lower <= 'z') {
        if (keyboard_shift_pressed ^ keyboard_caps_lock) return upper;
        return lower;
    }

    if (keyboard_shift_pressed) return upper;

    return lower;
}

registers_t* irq1_handler(registers_t* registers) {
    char c = keyboard_get_char();
    if (c) {
        uint32_t next = (keyboard_head + 1) % KEYBOARD_BUFFER_SIZE;
        if (next != keyboard_tail) {
            keyboard_buffer[keyboard_head] = c;
            keyboard_head = next;
        }
    }

    pic_send_eoi(1);
    return registers;
}

int keyboard_read_char(void) {
    if (keyboard_tail == keyboard_head) return -1;

    char c = keyboard_buffer[keyboard_tail];

    keyboard_tail = (keyboard_tail + 1) % KEYBOARD_BUFFER_SIZE;

    return (int)c;
}

const char keyboard_scancode_map_lower[128] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

const char keyboard_scancode_map_upper[128] = {
    0,   0,   '!', '"', 0, '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '~',
    0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    0,   ' ', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};