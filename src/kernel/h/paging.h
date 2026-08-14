#pragma once

#include "definitions.h"

class Paging {
    private:
        static uint32_t* page_directory;
        static uint32_t* page_table0;

        static void load_page_directory(uint32_t page_directory_address);
        static void enable_paging();
    public:
        Paging() = delete;

        static void initialise();
};