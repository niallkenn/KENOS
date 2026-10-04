#include "pit.h"
#include "portio.h"
#include "process.h"
#include "pic.h"
#include "syscall.h"
#include "scheduler.h"

volatile uint32_t timer_ticks = 0;

void init_pit(uint32_t frequency_hz) {
    uint32_t divisor = 1193182 / frequency_hz;

    outb(0x43, 0x36);

    outb(0x40, (uint8_t)divisor & 0xFF);
    outb(0x40, (uint8_t)(divisor >> 8) & 0xFF);
}

registers_t* irq0_handler(registers_t* registers) {
    timer_ticks++;

    pic_send_eoi(0);

    if (current_process == NULL || current_process == &idle_process) {
        return sys_yield(registers);
    }

    current_process->ticks_in_slice++;
    current_process->total_ticks++;

    if (current_process->ticks_in_slice >= time_slice[current_process->priority]) {
        current_process->ticks_in_slice = 0;

        if (current_process->priority < NUM_QUEUES - 1) {
            current_process->priority++;
        }

        return sys_yield(registers);
    }
    
    return registers;
}