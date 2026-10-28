#include "paging.h"
#include "memory/pmm.h"

// static function to load page directory into cr3
static void load_cr3(uint32_t pd_addr) {
    asm volatile(
        "mov %0, %%eax\n\t" // load address of pd into gen purpose first
        "mov %%eax, %%cr3" // then into cr3
        : // no outputs
        : "r" (pd_addr)
        : "eax", "memory" // tell compiler, modified eax and some other shi
    );
}

static void set_cr0_pg(void) {
    // take cr0
    uint32_t cr0;
    asm volatile(
        "mov %%cr0, %0"
        : "=r" (cr0)
    );

    // set bit 31, pg
    cr0 |= 0x80000000;

    // load cr0 with set pg back to cr0
    asm volatile(
        "mov %0, %%eax\n\t"
        "mov %%eax, %%cr0"
        :
        : "r" (cr0)
        : "eax", "memory"
    );
}

bool init_paging(void) {
    uint32_t page_directory_addr = frame_allocate();

    // check allocation
    if (page_directory_addr == 0) return false;

    uint32_t* page_directory_ptr = (uint32_t*)page_directory_addr;

    // initialise the 1024 pdes to NOT PRESENT, supervisor, read/write
    for (int i = 0; i < 1024; i++) {
        page_directory_ptr[i] = WRITABLE | SUPERVISOR;
    }

    // initially, only map the first 16 mb, so 4 page tables.
    // identity map
    for (int table = 0; table < 4; table++) {
        // actual page table
        uint32_t* page_table = (uint32_t*)frame_allocate();
        // check allocation
        if ((uint32_t)page_table == 0) return false;

        // fill the page table, supervisor only, writable, present
        for (int i = 0; i < 1024; i++) {
            // address we are mapping
            uint32_t phys_addr = (i * PAGE_SIZE) + (1024 * PAGE_SIZE * table);

            // apply flags to page table entry
            page_table[i] = phys_addr | SUPERVISOR | WRITABLE | PRESENT;
        }

        // put pte in page directory at index
        // permissions have to be applied at this stage too
        page_directory_ptr[table] = (uint32_t)page_table | SUPERVISOR | WRITABLE | PRESENT;
    }

    // load pd address into cr3#
    load_cr3(page_directory_addr);

    // enable cr0.pg paging enable bit
    set_cr0_pg();

    return true;
}