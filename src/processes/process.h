#ifndef PROCESSH
#define PROCESSH

#define MAX_PROCESSES 16

#include "definitions.h"

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

typedef struct process_t {
    int pid;
    process_state_t state;
} process_t;

extern process_t processes[MAX_PROCESSES];
extern int active_processes = 0;

process_t* process_create(void);
void process_exit(process_t* process);
int process_wait(process_t* process);
int process_exec(process_t* process);

#endif