#ifndef SYSCALLH
#define SYSCALLH

#include "interrupts.h"

// syscall names to number enum
typedef enum {
    SYS_WRITE = 1,
    SYS_YIELD = 2,
    SYS_GETPID = 3,
    SYS_EXIT = 4,
    MAX_SYSCALLS
} syscall_number_t;

extern int sys_write(uint32_t file_descriptor, const char* string, uint32_t length);
extern registers_t* sys_yield(registers_t* registers);
extern int sys_getpid(void);
extern registers_t* sys_exit();
#endif