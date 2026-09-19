#include "interrupts.h"
#include "syscall.h"

// interupt handler dispatcher definition
void interrupt_handler(registers_t* registers) {
    uint32_t vector = registers->interrupt_number;
    
    if (vector == 0x80) {
        handle_syscall(registers);
    }
    
    asm volatile("cli");
    for(;;) asm volatile("hlt");
}

void handle_syscall(registers_t* registers) {
    uint32_t syscall_number = registers->eax;
    
    if (syscall_number == 1) {
        sys_write(registers->ebx, (const char*)registers->ecx, registers->edx);
    }
}