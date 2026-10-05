#include "paging.h"
#include "frameallocator.h"

uint32_t* paging_page_directory = NULL;
uint32_t* paging_page_table_0 = NULL;

static void load_page_directory(uint32_t page_directory_address) {
    asm volatile(
        "movl %0, %%cr3"
        :
        : "r" (page_directory_address)
        : "memory"
    );
}

static void enable_paging(void) {
    uint32_t cr0;

    asm volatile(
        "mov %%cr0, %0"
        : "=r" (cr0)
    );

    cr0 |= 0x80000000;

    asm volatile(
        "mov %0, %%cr0"
        :
        : "r" (cr0)
        : "memory"
    );
}

void paging_map_page(uint32_t virtual_address, uint32_t physical_address, uint32_t flags) {
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FF;

    uint32_t* page_table;

    if ((paging_page_directory[page_directory_index] & PRESENT) == 0) {
        uint32_t table_physical = (uint32_t)(uintptr_t)fa_allocate();
        page_table = (uint32_t*)physical_to_virtual(table_physical);

        for (int i = 0; i < 1024; i++) page_table[i] = 0;

        paging_page_directory[page_directory_index] = table_physical | PRESENT | WRITABLE;
    } else {
        uint32_t table_phys = paging_page_directory[page_directory_index] & 0xFFFFF000;
        page_table = (uint32_t*)physical_to_virtual(table_phys);
    }

    page_table[page_table_index] = (physical_address & 0xFFFFF000) | flags;

    asm volatile(
        "invlpg (%0)"
        :
        : "r" (virtual_address)
        : "memory"
    );
}

void paging_unmap_page(uint32_t virtual_address) {
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FF;

    if ((paging_page_directory[page_directory_index] & PRESENT) == 0) return;

    uint32_t table_physical = paging_page_directory[page_directory_index] & 0xFFFFF000;
    uint32_t* page_table = (uint32_t*)physical_to_virtual(table_physical);

    if ((page_table[page_table_index] & PRESENT) == 0) return;

    fa_free((void*)(uintptr_t)(page_table[page_table_index] & 0xFFFFF000));

    page_table[page_table_index] = 0;

    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_address)
        : "memory"
    );
}

void init_paging(void) {
    paging_page_directory = (uint32_t*)fa_allocate();
    paging_page_table_0 = (uint32_t*)fa_allocate();

    for (int i = 0; i < 1024; i++) {
        paging_page_directory[i] = 0;
        paging_page_table_0[i] = 0;
    }

    for (int i = 0; i < 1024; i++) {
        uint32_t physical_address = i << 12;
        paging_page_table_0[i] = physical_address | PRESENT | WRITABLE;
    }

    uint32_t pt0_phys = virtual_to_physical(paging_page_table_0);
    paging_page_directory[0] = pt0_phys | PRESENT | WRITABLE;

    paging_page_directory[768] = pt0_phys | PRESENT | WRITABLE;

    uint32_t total_physical_bytes = fa_total_frames * PAGE_SIZE;
    uint32_t bytes_per_table = PAGE_SIZE * 1024;
    uint32_t num_tables = (total_physical_bytes + bytes_per_table - 1) / bytes_per_table;

    for (uint32_t t = 0; t < num_tables; t++) {
        uint32_t* table = (uint32_t*)fa_allocate();
        for (int i = 0; i < 1024; i++) {
            uint32_t physical = (t * 1024 + i) * PAGE_SIZE;
            table[i] = physical | PRESENT | WRITABLE;
        }

        uint32_t page_directory_index = (PHYS_MAP_BASE >> 22) + t;
        paging_page_directory[page_directory_index] = (uint32_t)(uintptr_t)table | PRESENT | WRITABLE;
    }

    uint32_t pd_phys = virtual_to_physical(paging_page_directory) & 0xFFFFF000;
    load_page_directory(pd_phys);

    enable_paging();
}