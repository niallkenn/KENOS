#ifndef KEYBOARDH
#define KEYBOARDH

#include "definitions.h"
#include "interrupts.h"

#define KEYBOARD_BUFFER_SIZE 256

extern const char keyboard_scancode_map_lower[128];
extern const char keyboard_scancode_map_upper[128];

extern bool keyboard_shift_pressed;
extern bool keyboard_caps_lock;
extern uint8_t keyboard_last_scancode;

char keyboard_get_char(void);
registers_t* irq1_handler(registers_t* registers);
int keyboard_read_char(void);

#endif