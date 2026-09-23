// includes
#include "idt.h"
#include "gdt.h"
#include "unistd.h"
#include "process.h"

extern void start_first_process(uint32_t esp);

void init_main(void) {
    while (1) {
        yield();
    }
}

void prog1(void) {
    const char* string = "pid"
    ;
    write(1, '0' + getpid(), 1);
    exit();
}

void prog2(void) {
    const char* string = "pid";
    write(1, string + '0' + getpid(), 4);
    exit();
}

// main kernel function
void kernel_main() {
    init_gdt();
    // init interrupt descriptor table
    init_idt();

    processes_init();

    process_t* init_proc = process_create(init_main);
    process_t* proc1 = process_create(prog1);
    process_t* proc2 = process_create(prog2);
    current_process = init_proc;

    tss_entry.esp0 = (uint32_t)init_proc->kernel_stack;
    
    start_first_process(init_proc->esp);

    // hold cpu
    while (1) asm volatile("hlt");
}