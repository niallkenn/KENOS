#include "screen.h"

void Screen::put_pixel(int x, int y, uint8_t color) {
            if (x < 0 || x >= 320 || y < 0 || y >= 200) return;

            vram[y * 320 + x] = color;
        }
        
void Screen::clear_screen(uint8_t color) {
    for (int i=0;i<320*200;i++) {
        vram[i] = color;
    }
}