#include "interrupts.h"
#include "panic.h"

extern "C" void interrupt_handler(uint32_t number) {
    switch (number) {
        case 0:
            panic("Error: division by zero");
    }
}