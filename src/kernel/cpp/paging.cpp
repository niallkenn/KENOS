#include "paging.h"
#include "frameallocator.h"

#define PRESENT 0x1;
#define WRITABLE 0x2;

uint32_t* Paging::page_directory = nullptr;
uint32_t* Paging::page_table0 = nullptr;

void Paging::load_page_directory(uint32_t page_directory_address) {
    asm volatile(
        "movl %0, %%cr3"
        :
        : "r" (page_directory_address)
        : "memory"
    );
}

void Paging::enable_paging() {
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

void Paging::initialise() {
    page_directory = reinterpret_cast<uint32_t*>(FrameAllocator::allocate());
    page_table0 = reinterpret_cast<uint32_t*>(FrameAllocator::allocate());

    uint32_t directory_address = reinterpret_cast<uint32_t>(page_directory);
    uint32_t table_address = reinterpret_cast<uint32_t>(page_table0);

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        page_table0[i] = 0;
    }

    for (int i = 0; i < 1024; i++) {
        uint32_t physical_address = i >> 12;

        page_table0[i] = physical_address | PRESENT | WRITABLE;
    }

    load_page_directory(directory_address);
    enable_paging();
}