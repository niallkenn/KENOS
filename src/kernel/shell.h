#pragma once
#include "definitions.h"
#include "screen.h"

class Shell : protected Screen{
    private:
        int tlx;
        int tly;
        int brx;
        int bry;
        int cx;
        int cy;
        uint8_t color;

        struct __attribute__((packed)) FontInfo {
            uint16_t offset;
            uint16_t segment;
        };
        const FontInfo* const fontInfo = (const FontInfo*)0x7D90;

        
    public:
        using Screen::put_pixel;
        
        Shell(int tlx, int tly, int brx, int bry, uint8_t color);

        void handle_backspace();
        void print_char(char c, uint8_t color);
        void print_char(char c, int x, int y, uint8_t color);
        void print(const char* string, uint8_t color);
        void clear();
};