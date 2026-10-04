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
        return irq0_handler(registers);
    } else if (irq == 1) {
        return irq1_handler(registers);
    }
    
    pic_send_eoi(irq);
    return registers;
}