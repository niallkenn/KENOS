#ifndef PROCESSH
#define PROCESSH

// max number of processes that can exist at once
#define MAX_PROCESSES 16

#include "definitions.h"

// different process states
typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

/* registers context for switching and pasing
process, to transfer between different states */
typedef struct cpu_context_t {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
    uint32_t eflags;
} cpu_context_t;

// process data
typedef struct process_t {
    int pid;
    process_state_t state;
    cpu_context_t context;
} process_t;

//global processes array
extern process_t processes[MAX_PROCESSES];
// number of active processes
extern int active_processes;

// process functions
process_t* process_create(void);
void process_exit(process_t* process);
int process_wait(process_t* process);
int process_exec(process_t* process);

#endif