#pragma once

#include "shell.h"

void k_clear(Shell& shell);
void k_echo(Shell& shell);
void k_help(Shell& shell);
void k_about(Shell& shell);
void k_mem(Shell& shell);
void k_calc(Shell& shell);
void k_ide(Shell& shell);

void k_command_error(Shell& shell);

void k_alloc(Shell& shell);
void k_panic();
