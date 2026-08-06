#include "terminal.h"
#include "screen.h"

Terminal::Terminal(int tlx, int tly, int brx, int bry, uint8_t fg_color, uint8_t bg_color) : 
     tlx(tlx), tly(tly), brx(brx), bry(bry), fg_color(fg_color), bg_color(bg_color) {
        clear();
}

void Terminal::backspace()
{
    if (cx == tlx && cy == tly)
        return;

    if (cx == tlx)
    {
        cy -= 8;
        cx = (((320 - 2 * tlx) / 8) * 8) + tlx;
    }
    else
    {
        cx -= 8;
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Screen::put_pixel(cx + col, cy + row, bg_color);
        }
    }
}

void Terminal::put_char(char c, uint8_t color) {
    if (c == '\n') {
        cy+=8;
        cx=tlx;
        return;
    }

    if (c == '\t') {
        put_char(' ', color);
        put_char(' ', color);
        return;
    }

    if (cx + 8 > brx) {
        cy+=8;
        cx = tlx;
    }
    
    draw_char(c, cx, cy);
    cx+=8;
}

void Terminal::draw_char(char c, int x, int y) const {
    if (x < tlx || (x + 8) > brx || y < tly || (y + 8) > bry) return;

    uint32_t fontBase = ((uint32_t)fontInfo->segment * 16) + fontInfo->offset;

    const uint8_t* glyph = (const uint8_t*)fontBase + ((uint8_t)c * 8);

    for (int row = 0; row < 8; row++) {
        uint8_t row_byte = glyph[row];

        for (int col = 0; col < 8; col++) {
            if (row_byte & (1 << (7 - col))) {
                Screen::put_pixel(x + col, y + row, fg_color);
            } else {
                Screen::put_pixel(x+col, y + row, bg_color);
            }
        }
    }
}

void Terminal::write(const char* string, uint8_t color) {
    int index = 0;
    while (string[index]) {
        put_char(string[index], color);
        index++;
    }
}

void Terminal::clear() {
    cx = tlx;
    cy = tly;

    for (int i = tlx; i < brx; i++) {
        for (int j = tly; j < bry; j++) {
            Screen::put_pixel(i, j, bg_color);
        }
    }
}

void Terminal::new_line() { put_char('\n', fg_color);}

int Terminal::get_cy() const { return cy;}

uint8_t Terminal::get_fg() const { return fg_color;}