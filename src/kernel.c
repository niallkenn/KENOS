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
#include "memorymap.h"
#include "frameallocator.h"
#include "paging.h"

extern void start_first_process(uint32_t esp);

multiboot_info_t* mb_info = NULL;

void init_main(void) {
    write(1, "HELLO FROM PID ", 15);
    char pid[12];
    itoa(getpid(), pid);
    write(1, pid, strlen(pid));
    write(1, "!\n", 2);

    exit();
}

// main kernel function
void kernel_main(uint32_t magic, multiboot_info_t* mbinfo) {
    if (magic != 0x2BADB002) return;

    if (mbinfo->flags & (1 << 12)) {
        mb_info = mbinfo;
    } else return;

    
    static uint32_t mmap_addr = 0;
    static uint32_t mmap_length = 0;
    if (mb_info->flags & (1 << 6)) {
        mmap_addr = mb_info->mmap_addr;
        mmap_length = mb_info->mmap_length;
    } else return;
    
    init_mmap(mmap_addr, mmap_length);
    
    init_gdt();
    init_idt();
    
    init_fa();

    init_paging();

    uint32_t fb_phys = (uint32_t)mb_info->framebuffer_addr;
    uint32_t fb_size = mb_info->framebuffer_pitch * mb_info->framebuffer_height;
    uint32_t fb_pages = (fb_size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (uint32_t i = 0; i < fb_pages; i++) {
        paging_map_page(fb_phys + i * PAGE_SIZE, fb_phys + i * PAGE_SIZE, PRESENT | WRITABLE);
    }

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