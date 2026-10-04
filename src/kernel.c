// includes
#include "idt.h"
#include "gdt.h"
#include "unistd.h"
#include "process.h"
#include "pic.h"
#include "pit.h"
#include "stdlib.h"
#include "scheduler.h"
#include "multiboot.h"
#include "terminal.h"

extern void start_first_process(uint32_t esp);

multiboot_info_t* mb_info = NULL;

void init_main(void) {
    write(1, "HELLO FROM PID ", 15);
    char pid[12];
    itoa(getpid(), pid);
    write(1, pid, strlen(pid));
    write(1, "!", 1);
    
    exit();
}

// main kernel function
void kernel_main(uint32_t magic, multiboot_info_t* mbinfo) {
    if (magic != 0x2BADB002) return;

    if (mbinfo->flags & (1 << 12)) {
        mb_info = mbinfo;
    } else return;

    init_gdt();
    // init interrupt descriptor table
    init_idt();
    pic_remap();
    init_pit(1000);

    processes_init();
    idle_create();

    process_t* init_proc = process_create(init_main);
    process_create(terminal_main);

    current_process = init_proc;
    tss_entry.esp0 = (uint32_t)init_proc->kernel_stack;

    start_first_process(init_proc->esp);

    // hold cpu
    while (1) asm volatile("hlt");
}