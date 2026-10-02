#include "scheduler.h"

const int time_slice[NUM_QUEUES] = {4, 8, 16};
static int ticks_since_boost = 0;
static int last_scheduled_index = -1;

process_t* scheduler_get_next() {
    ticks_since_boost++;
    if (++ticks_since_boost >= BOOST_INTERVAL) {
        ticks_since_boost = 0;
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (processes[i].state == PROCESS_READY || processes[i].state == PROCESS_RUNNING) {
                processes[i].priority = 0;
                processes[i].ticks_in_slice = 0;
            }
        }
    }

    for (int q = 0; q < NUM_QUEUES; q++) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            int idx = (last_scheduled_index + 1 + i) % MAX_PROCESSES;
            if (processes[idx].state == PROCESS_READY && processes[idx].priority == q) {
                last_scheduled_index = idx;
                return &processes[idx];
            }
        }
    }

    return &idle_process;
}