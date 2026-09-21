#include "process.h"

// initialise
process_t processes[MAX_PROCESSES];
uint8_t kernel_stacks[MAX_PROCESSES][STACK_SIZE];
uint8_t user_stacks[MAX_PROCESSES][STACK_SIZE];
int next_pid = 1;

process_t* current_process = NULL;

process_t* process_create(void (*entry_point)(void)) {
    int slot = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == UNUSED) {
            slot = i;
            break;
        }
    }
    if (slot == -1) return NULL;

    process_t* process = &processes[slot];
    process->pid = next_pid++;
    process->state = PROCESS_READY;

    uint8_t* kernel_stack = &kernel_stacks[slot][STACK_SIZE];
    uint8_t* user_stack = &user_stacks[slot][STACK_SIZE];

    process->kernel_stack = kernel_stack;

    // fake interrupt frame to iret into entry point
    uint32_t* sp = (uint32_t*)kernel_stack;

    // ss, esp, eflags, cs, eip
    *(--sp) = 0x23;
    *(--sp) = (uint32_t)user_stack;
    *(--sp) = 0x202;
    *(--sp) = 0x1B;
    *(--sp) = (uint32_t)entry_point;

    // interrupt vector and error code
    for (int i = 0; i < 2; i++) *(--sp) = 0;

    // eax, ecx, edx, ebx, esp, ebp, esi, edi
    for (int i = 0; i < 8; i++) *(--sp) = 0;

    // ds, es, fs, gs
    for (int i = 0; i < 4; i++) *(--sp) = 0x23;

    process->esp = (uint32_t)sp;

    return process;
}

void processes_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].state = UNUSED;
    }
}
