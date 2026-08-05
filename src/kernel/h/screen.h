#pragma once
#include "definitions.h"
class Screen {
    private:
        volatile uint8_t* const vram = (volatile uint8_t*)0xA0000;

    public:
        void put_pixel(int x, int y, uint8_t color);
        void clear_screen(uint8_t color);
};