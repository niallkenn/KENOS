#ifndef TERMINALH
#define TERMINALH

#include "definitions.h"

void terminal_put_char(char c, uint32_t fg, uint32_t bg);
void terminal_draw_char(char c, int x, int y, uint32_t fg, uint32_t bg);
void terminal_print(const char* str, uint32_t fg, uint32_t bg);
void terminal_clear(uint32_t bg);
void terminal_backspace(void);
void terminal_main(void);

#endif