#include "definitions.h"
#include "keyboard.h"
#include "portio.h"
#include "screen.h"
#include "shell.h"

class Keyboard {
    private:
        static constexpr char scancode_map[128] = {
            0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // 0x00 - 0x0E
            '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     // 0x0F - 0x1C
            0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',          // 0x1D - 0x28
            0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0,          // 0x29 - 0x35
            '*',    0, ' '
        };

        static inline uint8_t last_scancode = 0;

    public:
        static char get_char() {
            uint8_t scancode = PortIO::inb(0x60);

            if (scancode == last_scancode) return 0;
            last_scancode = scancode;

            if (scancode & 0x80) return 0;

            if (scancode < 128) {
                return scancode_map[scancode];
            } else return 0;
        }
};

class Screen {
    private:
        volatile uint8_t* const vram = (volatile uint8_t*)0xA0000;

    public:
        void put_pixel(int x, int y, uint8_t color) {
            if (x < 0 || x >= 320 || y < 0 || y >= 200) return;

            vram[y * 320 + x] = color;
        }
        
        void clear_screen(uint8_t color) {
            for (int i=0;i<320*200;i++) {
                vram[i] = color;
            }
        }
};

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
        
        Shell(int tlx, int tly, int brx, int bry, uint8_t color) : 
            tlx(tlx), tly(tly), brx(brx), bry(bry), color(color) {
                cx = tlx;
                cy = tly;
                clear();
        }

        void handle_backspace() {
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    put_pixel(cx + col, cy + row, color);
                }
            }
        }   
        
        void print_char(char c, uint8_t color) {
            if (c == '\n') {
                cy+=8;
                cx=tlx;
                return;
            }

            if (c == '\t') {
                print_char(' ', color);
                print_char(' ', color);
                return;
            }

            if (c == '\b') {
                if (cx - 8 < tlx && cy != tly) {
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
        
        void print_char(char c, int x, int y, uint8_t color) {
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

        void print(const char* string, uint8_t color) {
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

        void clear() {
            for (int x=tlx;x<brx;x++) {
                for (int y=tly;y<bry;y++) {              
                    put_pixel(x, y, color);           
                }
            }
        }
};

extern "C" void main() {
    Shell shell(50, 50, 270, 150, BLACK);

    shell.print(">", LIGHT_GREY);
    
    while (1) {
        char c = Keyboard::get_char();
        if (c!=0) shell.print_char(c, LIGHT_GREY);
    }
}
