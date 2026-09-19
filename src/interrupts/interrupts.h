#ifndef INTERRUPTSH
#define INTERRUPTSH

#include "definitions.h"

// registers frame pushed after interrupt
typedef struct registers_t {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t interrupt_number;
    uint32_t error_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} __attribute__((packed)) registers_t;

// common interrupt handler dispatcher
void interrupt_handler(registers_t* registers);

// syscall dispatcher
extern void handle_syscall(registers_t* registers);

#endif