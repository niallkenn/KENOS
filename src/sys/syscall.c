#include "syscall.h"
#include "process.h"
#include "gdt.h"
#include "scheduler.h"
#include "terminal.h"

extern int sys_write(uint32_t file_descriptor, const char* string, uint32_t length) {
    if (file_descriptor == 1 || file_descriptor == 2) {
        for (uint32_t i = 0; i < length; i++) {
            put_char(string[i], WHITE, BLACK);
        }
        return length;
    }   

    return -1;
}

extern registers_t* sys_yield(registers_t* registers) {
    if (current_process != NULL && current_process != &idle_process) {
        current_process->state = PROCESS_READY;
        current_process->esp = (uint32_t)registers;
    }

    process_t* next_process = scheduler_get_next();

    current_process = next_process;
    current_process->state = PROCESS_RUNNING;

    tss_entry.esp0 = (uint32_t)next_process->kernel_stack;

    return (registers_t*)next_process->esp;
}

extern int sys_getpid(void) {
    return current_process->pid;
}

extern registers_t* sys_exit() {
    current_process->state = PROCESS_TERMINATED;

    process_t* next_process = scheduler_get_next();

    current_process = next_process;

    tss_entry.esp0 = (uint32_t)next_process->kernel_stack;

    return (registers_t*)next_process->esp;
}