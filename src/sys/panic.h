#include "shell.h"
#include "interrupts.h"

[[noreturn]] void panic(const char* message, Registers* regs);