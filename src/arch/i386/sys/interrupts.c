#include "interrupts.h"
#include "pic.h"

volatile uint64_t timer_ticks = 0;

void handle_exception(uint32_t vector) {
    int index = 0;

    // vga framebuffer
    volatile unsigned char* fb = (volatile unsigned char*)0xb8000;

    const char* message = "ERROR, exception ";

    // print message
    for (int i = index; i < strlen(message); i++) {
        fb[i*2] = message[i];
        index++;
    }

    char vec[2];
    itoa(vector, vec);

    // print exception number
    for (int i = index; i < index + 2; i++) {
        fb[i*2] = vec[i - index];
    }
}

void handle_irq(uint32_t vector) {
    int irq = vector - 32;

    switch (irq) {
        case 0: {
            timer_ticks++;
            pic_send_eoi(0);
            break;
        }
    }
}