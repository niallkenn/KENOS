#pragma once

#include "definitions.hpp"

#define PRESENT  0x01
#define WRITABLE 0x02
#define USER     0x04

class Paging {
private:
    static uint32_t* page_directory;

    static void load_page_directory(uint32_t page_directory_address);
    static void enable_paging();

public:
    Paging() = delete;

    static void initialise();
    static void map_page(uint32_t virtual_address, uint32_t physical_address, uint32_t flags);
    static void unmap_page(uint32_t virtual_address);
};