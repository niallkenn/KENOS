#include "tss.h"

// init global tss_entry
struct tss_entry tss_entry = {};

// boot stack top pointer in boot.S
extern uint8_t* boot_stack_top;

void init_tss(void) {
    // 0 init the tss structure
    uint8_t* p = (uint8_t*)&tss_entry;
    int size = sizeof(tss_entry);
    while (size--) {
        *p = 0;
        p++;
    }

    // init ss0, esp0 and iomap_base. 
    // ss0 is the kernel data selector (2*8) | 0 | 0 = 0x10
    tss_entry.ss0 = 0x10;
    // the kernel stack set first should be the boot stack, initially before first switch to ring 3
    tss_entry.esp0 = (uint32_t)&boot_stack_top;
    // the iomap base addrsss of bitmap. equal or outside the end of the tss means no ring3 priveleges for I/O
    tss_entry.iomap_base = sizeof(tss_entry);

    // load task register with tss selector
    init_tss_asm();
}