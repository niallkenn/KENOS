#include "scheduler.h"

static int current_index = 0;

process_t* scheduler_get_next() {
    // Start searching from the next slot in the array
    int index = (current_index + 1) % MAX_PROCESSES;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        int check_slot = (index + i) % MAX_PROCESSES;
        
        if (processes[check_slot].state == PROCESS_READY) {
            current_index = check_slot; // Remember this slot for next time
            return &processes[check_slot];
        }
    }

    // Fallback: If no other process is ready, try to keep running the current one
    if (current_process && current_process->state == PROCESS_READY) {
        return current_process;
    }

    // Ultimate fallback: Everything is blocked (you'll need an idle process here later!)
    return NULL; 
}