#ifndef VMM_H
#define VMM_H

#include "user/definitions.h"

// maps a virtual address of physical page aligned frame, to physical address
bool vmm_map_page(uint32_t virt_addr, uint32_t phys_addr, uint32_t flags);

#endif