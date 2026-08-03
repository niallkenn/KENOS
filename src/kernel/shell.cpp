#include "shell.h"

Shell init_shell(int tlx, int tly, int brx, int bry, uint8_t fg_color, uint8_t bgcolor) {
    Shell shell(tlx, tly, brx, bry, bgcolor);
    shell.clear_screen(bgcolor);
    shell.print_char('>', fg_color);
    return shell;
}

void Shell::handle_backspace() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            put_pixel(cx + col, cy + row, color);
        }
    }
}   

Shell::Shell(int tlx, int tly, int brx, int bry, uint8_t color) : 
    tlx(tlx), tly(tly), brx(brx), bry(bry), color(color) {
        cx = tlx;
        cy = tly;
        clear();
}

Shell& Shell::operator=(const Shell& other) {
    if (this != &other) {
        tlx = other.tlx;
        tly = other.tly;
        brx = other.brx;
        bry = other.bry;
        color = other.color;
    }

    return *this;
}

Shell::Shell(const Shell& other) {
    tlx = other.tlx;
    tly = other.tly;
    brx = other.brx;
    bry = other.bry;
    color = other.color;
}
        
void Shell::print_char(char c, uint8_t color) {
    if (c == '\n') {
        cy+=8;
        cx=tlx;
        print_char('>', color);
        last_arrow_y = cy;
        return;
    }

    if (c == '\t') {
        print_char(' ', color);
        print_char(' ', color);
        return;
    }

    if (c == '\b') {
        if (last_arrow_y == cy && cx - 8 == tlx) {
            return;
        } else if (cx - 8 < tlx && cy != tly) {
            cy -= 8;
            cx = (((320 - 2 * tlx) / 8) * 8) + tlx;
        } else if (cx - 8 < tlx && cy == tly) {
            handle_backspace();
            return;
        }

        cx-=8;
        handle_backspace();
        return;
    }

    if (cx + 8 > brx) {
        cy+=8;
        cx = tlx;
    }
    
    print_char(c, cx, cy, color);
    cx+=8;
}

void Shell::print_char(char c, int x, int y, uint8_t color) {
    if (x < tlx || (x + 8) > brx || y < tly || (y + 8) > bry) return;

    uint32_t fontBase = ((uint32_t)fontInfo->segment * 16) + fontInfo->offset;

    const uint8_t* glyph = (const uint8_t*)fontBase + ((uint8_t)c * 8);

    for (int row = 0; row < 8; row++) {
        uint8_t row_byte = glyph[row];

        for (int col = 0; col < 8; col++) {
            if (row_byte & (1 << (7 - col))) {
                put_pixel(x + col, y + row, color);
            }
        }
    }
}

void Shell::print(const char* string, uint8_t color) {
    int index = 0;
    while (string[index]) {
        if (string[index] == '\n') {
            cy += 8;
            cx = tlx;
            index++;
        }

        if (cx + 8 > brx) {
            cy+=8;
            cx = tlx;
        }

        print_char(string[index], cx, cy, color);
        index++;
        cx+=8;
    }
}

void Shell::print(const String& str, uint8_t color) {
    print(str.c_str(), color);
}

void Shell::clear() {
    for (int x=tlx;x<brx;x++) {
        for (int y=tly;y<bry;y++) {              
            put_pixel(x, y, color);           
        }
    }
}