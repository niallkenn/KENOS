#pragma once

#include "definitions.h"

class Terminal {
    private:
        int cx, cy;

        int tlx, tly;
        int brx, bry;

        uint8_t fg_color;
        uint8_t bg_color;

        struct __attribute__((packed)) FontInfo {
            uint16_t offset;
            uint16_t segment;
        };

        const FontInfo* const fontInfo = (const FontInfo*)0x7D90;

        void draw_char(char c, int x, int y) const;
    public:
        Terminal(int tlx, int tly, int brx, int bry,  uint8_t fg_color, uint8_t bg_color);

        void put_char(char c, uint8_t color);

        void write(const char* string, uint8_t color);

        int get_cy() const;
        uint8_t get_fg() const;

        void clear();

        void backspace();
        void new_line();
};