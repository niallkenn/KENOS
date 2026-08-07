#include "irqhandlers.h"
#include "pit.h"

void handle_irq0() {
    timer_ticks++;
}