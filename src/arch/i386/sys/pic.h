#ifndef PIC_H
#define PIC_H

#include "user/definitions.h"

// pic port definitions
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20

// Initialisation command words (ICW) definitions.
#define ICW1_INIT 0x10 // base initialisation
#define ICW1_ICW4 0x01 // indicates ICW4 will be present
#define PIC1_OFFSET 32
#define PIC2_OFFSET 40
#define CASCADE_IRQ 2 // irq that links slave to master
#define ICW4_8086 0x01 // 8086 pic mode, no idea what that is but osdev wiki yes

// init pic, which remaps it, adds offset to irq vectors for slave and master
void init_pic(void);
// sends eoi to pic, indicating the irq handler has finished
void pic_send_eoi(uint8_t irq);

#endif