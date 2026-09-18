#include "idt.h"

// init
idt_entry_t idt[256] = {};
idtr_t idtr;

// load idt function
void load_idt(idtr_t* idtr) {
    asm volatile(
        "lidt %0"
        :
        : "m" (*idtr)
    );
}

// initialse idt
void init_idt(){
    // assign idtr values
    idtr.base = (uint32_t)idt;
    idtr.limit = sizeof(idt) - 1;

    // load the idtr
    load_idt(&idtr);
}