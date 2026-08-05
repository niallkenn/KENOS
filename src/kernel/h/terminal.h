#pragma once

#include "definitions.h"

class Terminal {
    private:
        int cx, cy;

        int brx, bry;
        int tlx, tly;

        uint8_t bg_color;
        uint8_t fg_color;

        struct __attribute__((packed)) FontInfo {
            uint16_t offset;
            uint16_t segment;
        };

        const FontInfo* const fontInfo = (const FontInfo*)0x7D90;

        void draw_char(char c, int x, int y, uint8_t color);
    public:
        Terminal();

        void put_char(char c, uint8_t color);

        void write(const char* string, uint8_t color);

        void clear();
};