#ifndef SYSCALLH
#define SYSCALLH

#include "interrupts.h"

// syscall names to number enum
typedef enum {
    SYS_WRITE = 1,
    MAX_SYSCALLS
} syscall_number_t;

extern int sys_write(uint32_t file_descriptor, const char* string, uint32_t length);

#endif