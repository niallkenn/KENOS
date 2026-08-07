#include "interrupts.h"
#include "panic.h"

const char* exception_messages[32] =
{
    "Divide Error",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

void enable_interrupts() {
    asm volatile("sti");
}

extern "C" void interrupt_handler(Registers* regs) {
    uint32_t number = regs->interrupt_number;
    if (number < 32) {
        panic(exception_messages[number], regs);
    } else if (number >= 32 && number <= 47) {
        switch (number)
        {
        case 3:
            
            break;
        
        default:
            break;
        }
    } else {
        panic("Unhandled or Reserved Exception Induced\n", regs);
    }
}