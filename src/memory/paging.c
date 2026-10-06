#include "paging.h"
#include "frameallocator.h"

uint32_t *paging_page_directory = NULL;
uint32_t *paging_page_table_0 = NULL;

static void load_page_directory(uint32_t page_directory_address) {
    asm volatile(
        "movl %0, %%cr3"
        :
        : "r"(page_directory_address)
        : "memory"
    );
}

static void enable_paging(void) {
    uint32_t cr0;

    asm volatile(
        "mov %%cr0, %0"
        : "=r"(cr0)
    );

    cr0 |= 0x80000000u;

    asm volatile(
        "mov %0, %%cr0"
        :
        : "r"(cr0)
        : "memory"
    );
}

void paging_map_page(uint32_t virtual_address,
                     uint32_t physical_address,
                     uint32_t flags) {
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FFu;
    uint32_t *page_table;

    if ((paging_page_directory[page_directory_index] & PRESENT) == 0) {
        void *table = fa_allocate();
        if (table == NULL) return;

        uint32_t table_physical = (uint32_t)(uintptr_t)table;

        /*
         * This function is called after init_paging(), so the physical
         * address is accessible through the physical-memory mapping.
         */
        page_table = (uint32_t *)physical_to_virtual(table_physical);

        for (uint32_t i = 0; i < 1024; i++)
            page_table[i] = 0;

        paging_page_directory[page_directory_index] =
            table_physical | PRESENT | WRITABLE;

        /*
         * Changing a PDE can affect translation for this address.
         * Reloading CR3 is simple and correct here.
         */
        load_page_directory(
            (uint32_t)(uintptr_t)paging_page_directory);
    } else {
        uint32_t table_physical =
            paging_page_directory[page_directory_index] & 0xFFFFF000u;

        page_table = (uint32_t *)physical_to_virtual(table_physical);
    }

    page_table[page_table_index] =
        (physical_address & 0xFFFFF000u) | (flags & 0xFFFu);

    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_address)
        : "memory"
    );
}

void paging_unmap_page(uint32_t virtual_address) {
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FFu;

    if ((paging_page_directory[page_directory_index] & PRESENT) == 0)
        return;

    uint32_t table_physical =
        paging_page_directory[page_directory_index] & 0xFFFFF000u;
    uint32_t *page_table =
        (uint32_t *)physical_to_virtual(table_physical);

    if ((page_table[page_table_index] & PRESENT) == 0)
        return;

    /*
     * Do NOT free the physical frame here. A page mapping does not
     * imply ownership of the physical frame, and the page may point
     * at kernel/firmware/device memory.
     */
    page_table[page_table_index] = 0;

    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_address)
        : "memory"
    );
}

void init_paging(void) {
    void *pd = fa_allocate();
    void *pt0 = fa_allocate();

    if (pd == NULL || pt0 == NULL)
        return;

    /*
     * These are physical addresses, but the initial page table is an
     * identity map, so they can be accessed directly before paging is
     * enabled.
     */
    paging_page_directory = (uint32_t *)(uintptr_t)pd;
    paging_page_table_0 = (uint32_t *)(uintptr_t)pt0;

    for (uint32_t i = 0; i < 1024; i++) {
        paging_page_directory[i] = 0;
        paging_page_table_0[i] = 0;
    }

    /* Identity-map the first 4 MiB. */
    for (uint32_t i = 0; i < 1024; i++) {
        uint32_t physical_address = i << 12;
        paging_page_table_0[i] =
            physical_address | PRESENT | WRITABLE | USER;
    }

    uint32_t pt0_phys = (uint32_t)(uintptr_t)pt0;

    paging_page_directory[0] =
        pt0_phys | PRESENT | WRITABLE | USER;

    /*
     * Temporary high mapping of the first 4 MiB. This is retained
     * because PHYS_MAP_BASE is 0xC0000000.
     */
    paging_page_directory[PHYS_MAP_PDE] =
        pt0_phys | PRESENT | WRITABLE;

    uint64_t total_physical_bytes =
        (uint64_t)fa_total_frames * PAGE_SIZE;
    uint64_t bytes_per_table = 4ULL * 1024ULL * 1024ULL;
    uint32_t num_tables =
        (uint32_t)((total_physical_bytes + bytes_per_table - 1) /
                   bytes_per_table);

    /*
     * PHYS_MAP_BASE leaves only 256 PDEs for the physical-memory
     * mapping, i.e. 1 GiB with a 3 GiB base.
     */
    if (num_tables > PHYS_MAP_PDE_COUNT)
        num_tables = PHYS_MAP_PDE_COUNT;

    for (uint32_t t = 0; t < num_tables; t++) {
        void *table_physical_ptr = fa_allocate();
        if (table_physical_ptr == NULL)
            return;

        uint32_t table_physical =
            (uint32_t)(uintptr_t)table_physical_ptr;

        /*
         * We are still running with the identity map active, so the
         * newly allocated page-table frame can be written directly.
         */
        uint32_t *table =
            (uint32_t *)(uintptr_t)table_physical;

        for (uint32_t i = 0; i < 1024; i++) {
            uint64_t physical =
                ((uint64_t)t * 1024ULL + i) * PAGE_SIZE;

            if (physical >= total_physical_bytes) {
                table[i] = 0;
            } else {
                table[i] =
                    (uint32_t)physical | PRESENT | WRITABLE;
            }
        }

        paging_page_directory[PHYS_MAP_PDE + t] =
            table_physical | PRESENT | WRITABLE;
    }

    uint32_t pd_phys =
        (uint32_t)(uintptr_t)pd & 0xFFFFF000u;

    load_page_directory(pd_phys);
    enable_paging();
}
