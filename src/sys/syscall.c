#include "syscall.h"
#include "process.h"
#include "gdt.h"
#include "scheduler.h"

static uint32_t index = 0;
extern int sys_write(uint32_t file_descriptor, const char* string, uint32_t length) {
    if (file_descriptor == 1 || file_descriptor == 2) {
        volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
        for (uint32_t i = 0; i < length; i++) {
            vga[index++] = (0x0F << 8) | string[i];
        }
        return length;
    }   

    return -1;
}

extern registers_t* sys_yield(registers_t* registers) {
    current_process->state = PROCESS_READY;
    current_process->esp = (uint32_t)registers;
    process_t* next_process = scheduler_get_next();
    if (next_process == NULL) return registers;

    current_process = next_process;

    tss_entry.esp0 = (uint32_t)next_process->kernel_stack;

    return (registers_t*)next_process->esp;
}

extern int sys_getpid(void) {
    return current_process->pid;
}

extern registers_t* sys_exit() {
    current_process->state = PROCESS_TERMINATED;

    process_t* next_process = scheduler_get_next();

    if (next_process == NULL || next_process == current_process) {
        volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
        vga[0] = (0x4F << 8) | 'E'; // Red background 'E' for Error/Empty
        asm volatile("cli");
        for(;;) asm volatile("hlt");
    }

    current_process = next_process;

    tss_entry.esp0 = (uint32_t)next_process->kernel_stack;

    return (registers_t*)next_process->esp;
}