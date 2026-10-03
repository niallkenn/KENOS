#include "irq.h"
#include "pic.h"
#include "scheduler.h"
#include "pit.h"
#include "unistd.h"
#include "syscall.h"
#include "keyboard.h"
#include "terminal.h"

registers_t* handle_irq(registers_t* registers) {
    int irq = registers->interrupt_number - 32;

    if (irq == 0) {
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
    } else if (irq == 1) {
        char c = keyboard_get_char();

        if (c) {
            if (c == '\b') {
                terminal_backspace();
            } else {
                terminal_put_char(c, WHITE, BLACK);
            }
        }
        pic_send_eoi(1);
        return registers;
    }
    
    pic_send_eoi(irq);
    return registers;
}