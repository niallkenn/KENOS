#include "pic.h"
#include "arch/i386/sys/io.h"

// static io wait, called between each hardware port write
static void io_wait(void) {
    // holds io ports for roughly 1 microsecond
    outb(0x80, 0);
}

// init pic, remap irqs. io wait between each out, allows time to perform the out
void init_pic(void) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // icw1, init mode master, icw4 will be present
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); // icw1, init mode slave, icw4 present
    io_wait();
    outb(PIC1_DATA, PIC1_OFFSET); // icw2, master offset
    io_wait();
    outb(PIC2_DATA, PIC2_OFFSET); // icw2, slave offset
    io_wait();
    outb(PIC1_DATA, 1 << CASCADE_IRQ); // icw3, tell master pic that theres slave at irq 2 (cascade irq)
    io_wait();
    outb(PIC2_DATA, CASCADE_IRQ); // icw3, tell slave pic its cascade identity
    io_wait();
    outb(PIC1_DATA, ICW4_8086); // icw4, 8086 mode (idk)
    io_wait();
    outb(PIC2_DATA, ICW4_8086); // icw4, 8086 mode (idk again)
    io_wait();

    // unmask pics (idk idk)
    outb(PIC1_DATA, 0);
    io_wait();
    outb(PIC2_DATA, 0);
}

void pic_send_eoi(uint8_t irq) {
    // only send eoi to both if irq is slave, irq2 of master gets called if so, has to be sent eoi
    if (irq >= 8) outb(PIC2_COMMAND, PIC_EOI);

    // send eoi to master pic
    outb(PIC1_COMMAND, PIC_EOI);
}