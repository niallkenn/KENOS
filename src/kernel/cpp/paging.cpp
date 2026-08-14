#include "paging.h"
#include "frameallocator.h"

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
    uint32_t table0_address = reinterpret_cast<uint32_t>(page_table0);

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        page_table0[i] = 0;
    }

    for (int i = 0; i < 1024; i++) {
        uint32_t physical_address = i << 12;

        page_table0[i] = physical_address | PRESENT | WRITABLE;
    }

    page_directory[0] = table0_address | PRESENT | WRITABLE;

    load_page_directory(directory_address);
    enable_paging();
}

void Paging::map_page(uint32_t virtual_address, uint32_t physical_address, uint32_t flags) {
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FF;

    uint32_t* page_table = nullptr;

    if ((page_directory[page_directory_index] & PRESENT) == 0) {
        page_table = reinterpret_cast<uint32_t*>(FrameAllocator::allocate());

        for (int i = 0; i < 1024; i++) page_table[i] = 0;

        page_directory[page_directory_index] = reinterpret_cast<uint32_t>(page_table) | PRESENT | WRITABLE;
    } else {
        page_table = reinterpret_cast<uint32_t*>(page_directory[page_directory_index] & 0xFFFFF000);
    }

    page_table[page_table_index] = (physical_address & 0xFFFFF000) | flags;

    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_address)
        : "memory"
    );
}

void Paging::unmap_page(uint32_t virtual_address) {
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FF;

    
    if ((page_directory[page_directory_index] & PRESENT) == 0) return;
    
    uint32_t* page_table = reinterpret_cast<uint32_t*>(page_directory[page_directory_index] & 0xFFFFF000);

    if ((page_table[page_table_index] & PRESENT) == 0) return;

    FrameAllocator::free(reinterpret_cast<void*>(page_table[page_table_index] & 0xFFFFF000));

    page_table[page_table_index] = 0;

    asm volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_address)
        : "memory"
    );
}