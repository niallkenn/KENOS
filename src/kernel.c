// includes
#include "idt.h"
#include "gdt.h"
#include "unistd.h"

extern void enter_usermode(uint32_t entry_point, uint32_t user_stack);

static uint8_t user_stack[4096];

void user_main(void) {
    const char* msg = "funny 67";
    
    write(1, msg, 9);

    while (1) {}
}
// main kernel function
void kernel_main() {
    init_gdt();
    // init interrupt descriptor table
    init_idt();

    

    //volatile uint16_t *vga = (volatile uint16_t *)0xB8000;
    //vga[0] = (0x0F << 8) | '1';

    uint32_t user_esp = (uint32_t)user_stack + sizeof(user_stack);

    enter_usermode((uint32_t)&user_main, user_esp);

    // hold cpu
    while (1) asm volatile("hlt");
}