// includes
#include "idt.h"
#include "gdt.h"
#include "unistd.h"
#include "process.h"

extern void start_first_process(uint32_t esp);

void init_main(void) {
    write(1, "HELLO FROM PID 1, INIT ", 24);
    
    while (1) {yield();}
}

void please_work(void) {
    while (1) {
        write(1, "1", 2);
        yield();
    }
}

void work2(void) {
    while (1) {
        write(1, "2", 2);
        yield();
    }
}
// main kernel function
void kernel_main() {
    init_gdt();
    // init interrupt descriptor table
    init_idt();

    processes_init();

    process_t* init_proc = process_create(init_main);
    process_t* please = process_create(please_work);
    process_t* please2 = process_create(work2);
    current_process = init_proc;

    tss_entry.esp0 = (uint32_t)init_proc->kernel_stack;
    
    start_first_process(init_proc->esp);

    // hold cpu
    while (1) asm volatile("hlt");
}