#ifndef UNISTDH
#define UNISTDH

#include "interrupts.h"

extern int write(uint32_t file_descriptor, const char* string, uint32_t length);

#endif