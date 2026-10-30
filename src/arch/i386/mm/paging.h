#ifndef PAGING_H
#define PAGING_H

#include "user/definitions.h"

// definitions for flags in ptes, pdes, sizes etc
#define PRESENT 0b1
#define WRITABLE 0b10
#define USER 0b100
#define SUPERVISOR 0b000

#define PAGE_SIZE 4096

// global page directory address
extern uint32_t page_directory;

// init paging and enable paging, cr3 and cr0.pg
bool init_paging(void);

#endif  