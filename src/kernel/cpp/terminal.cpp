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
        cx = tlx + ((brx - tlx) / 8 - 1) * 8;
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

void Terminal::put_char(char c) {
    if (c == '\n') {
        cy+=8;
        cx=tlx;
        return;
    }

    if (c == '\t') {
        put_char(' ');
        put_char(' ');
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

void Terminal::write(const char* string) {
    int index = 0;
    while (string[index]) {
        put_char(string[index]);
        index++;
    }
}

void Terminal::write_uint(uint32_t number) {
    char buffer[11];

    int i = 10;
    buffer[i] = '\0';

    if (number == 0) {
        write("0");
        return;
    }

    while (number > 0) {
        i--;
        buffer[i] = '0' + (number % 10);
        number /= 10;
    }

    write(&buffer[i]);
}

void Terminal::write_hex(uint32_t number) {
    const char* hex_chars = "0123456789ABCDEF";

    char buffer[9];
    buffer[8] = '\0';

    for (int i = 7; i >= 0; --i) {
        buffer[i] = hex_chars[number & 0xF];
        number >>= 4;
    }

    write("0x");
    write(buffer);
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

void Terminal::new_line() { put_char('\n');}

int Terminal::get_cy() const { return cy;}