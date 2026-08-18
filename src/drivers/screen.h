#pragma once
#include "definitions.h"
class Screen {
    private:
        static volatile uint8_t* const vram;
    
    public:
        static void put_pixel(int x, int y, uint8_t color);
        static void clear_screen(uint8_t color);

        Screen() = delete;
};