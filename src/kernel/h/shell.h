#pragma once
#include "definitions.h"
#include "screen.h"

class Shell {
    private:
        int tlx, tly;
        int brx, bry;

        int cx, cy;

        uint8_t bg_color;
        uint8_t fg_color;

        int last_arrow_y = tly;

        struct __attribute__((packed)) FontInfo {
            uint16_t offset;
            uint16_t segment;
        };
        const FontInfo* const fontInfo = (const FontInfo*)0x7D90;

        
    public:
        Shell(int tlx, int tly, int brx, int bry, uint8_t fg_color, uint8_t bg_color);

        Shell& operator=(const Shell& other);
        Shell(const Shell& other);

        void handle_enter();
        void handle_backspace();
        void print_char(char c, uint8_t color);
        void print_char(char c, int x, int y, uint8_t fg_color);
        void print(const char* string, uint8_t color);
        void clear();
};

Shell init_shell(int tlx, int tly, int brx, int bry, uint8_t fg_color, uint8_t bgcolor);
