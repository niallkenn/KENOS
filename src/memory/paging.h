#ifndef PAGINGH
#define PAGINGH

#include "definitions.h"

#define PRESENT  0x001u
#define WRITABLE 0x002u
#define USER     0x004u

#define PHYS_MAP_BASE 0xC0000000u
#define PHYS_MAP_PDE  (PHYS_MAP_BASE >> 22)
#define PHYS_MAP_PDE_COUNT (1024u - PHYS_MAP_PDE)

extern uint32_t *paging_page_directory;
extern uint32_t *paging_page_table_0;

void init_paging(void);
void paging_map_page(uint32_t virtual_address,
                     uint32_t physical_address,
                     uint32_t flags);
void paging_unmap_page(uint32_t virtual_address);

static inline void *physical_to_virtual(uint32_t physical) {
    return (void *)(uintptr_t)(physical + PHYS_MAP_BASE);
}

static inline uint32_t virtual_to_physical(void *virtual_address) {
    return (uint32_t)(uintptr_t)virtual_address - PHYS_MAP_BASE;
}

#endif
