// includes
#include "idt.h"
#include "gdt.h"
#include "unistd.h"
#include "process.h"
#include "pic.h"
#include "pit.h"
#include "stdlib.h"
#include "scheduler.h"

extern void start_first_process(uint32_t esp);

void init_main(void) {
    write(1, "HELLO FROM PID ", 15);
    char pid[12];
    itoa(getpid(), pid);
    write(1, pid, strlen(pid));
    write(1, "!", 1);
    
    exit();
}

void hello(void)  { while (1) { write(1, "1", 1); for (volatile int i=0;i<2000000;i++); } }
void hello2(void) { while (1) { write(1, "2", 1); for (volatile int i=0;i<2000000;i++); } }

// main kernel function
void kernel_main() {
    init_gdt();
    // init interrupt descriptor table
    init_idt();
    pic_remap();
    init_pit(1000);

    processes_init();
    idle_create();

    process_t* init_proc = process_create(init_main);
    process_create(hello);
    process_create(hello2);

    current_process = init_proc;
    tss_entry.esp0 = (uint32_t)init_proc->kernel_stack;

    start_first_process(init_proc->esp);

    // hold cpu
    while (1) asm volatile("hlt");
}