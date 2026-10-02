#ifndef TERMINALH
#define TERMINALH

#include "definitions.h"

void put_char(char c, uint32_t fg, uint32_t bg);
void print(const char* str, uint32_t fg, uint32_t bg);
void clear(uint32_t bg);

#endif