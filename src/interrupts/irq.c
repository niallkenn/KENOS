#include "irq.h"
#include "pic.h"
#include "scheduler.h"
#include "pit.h"
#include "unistd.h"
#include "syscall.h"

registers_t* handle_irq(registers_t* registers) {
    int irq = registers->interrupt_number - 32;
    if (irq == 0) {
        timer_ticks++;
        pic_send_eoi(0);
        return sys_yield(registers);
    }
    pic_send_eoi(irq);
    return registers;
}