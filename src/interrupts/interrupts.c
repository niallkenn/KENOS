#include "interrupts.h"
#include "syscall.h"
#include "process.h"

// interupt handler dispatcher definition
registers_t* interrupt_handler(registers_t* registers) {
    uint32_t vector = registers->interrupt_number;

    if (current_process != NULL) {
        current_process->esp = (uint32_t)registers;
    }

    registers_t* return_registers = registers;
    
    if (vector == 0x80) {
        return_registers = handle_syscall(registers);
    }

    if (vector < 32) {
        // exception
    }
    
    return return_registers;
}

registers_t* handle_syscall(registers_t* registers) {
    uint32_t syscall_number = registers->eax;
    
    if (syscall_number == SYS_WRITE) {
        registers->eax = sys_write(registers->ebx, (const char*)registers->ecx, registers->edx);
        return registers;
    } else if (syscall_number == SYS_YIELD) {
        return sys_yield(registers);
    } else if (syscall_number == SYS_GETPID) {
        registers->eax = sys_getpid();
        return registers;
    } else if (syscall_number == SYS_EXIT) {
        return sys_exit(registers);
    }

    return registers;
}