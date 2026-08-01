typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

enum VgaColor : uint8_t {
    BLACK         = 0,
    DARK_BLUE          = 1,
    DARK_GREEN         = 2,
    CYAN          = 3,
    DARK_RED           = 4,
    MAGENTA       = 5,
    BROWN         = 6,
    LIGHT_GREY    = 7,
    DARK_GREY     = 8,
    PURPLE    = 9,
    LIGHT_GREEN   = 10,
    LIGHT_BLUE    = 11,
    LIGHT_RED     = 12,
    PINK = 13,
    YELLOW        = 14,
    WHITE         = 15
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

        void print_char(char c, int x, int y, uint8_t color) {
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

    public:
        using Screen::put_pixel;

        Shell(int tlx, int tly, int brx, int bry, uint8_t color) : 
            tlx(tlx), tly(tly), brx(brx), bry(bry), color(color) {
                cx = tlx;
                cy = tly;
                clear();
        }

        void print_char(char c, uint8_t color) {
            if (cx + 8 > brx) {
                    cy+=8;
                    cx = tlx;
                }

                print_char(c, cx, cy, color);
                cx+=8;
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
            for (int x=0;x<320;x++) {
                for (int y=0;y<200;y++) {
                    if (!(x < tlx || x >= brx || y < tly || y >= bry)) {
                        put_pixel(x, y, color);
                    }
                }
            }
        }
};

extern "C" void main() {
    Shell shellb(0, 0, 158, 200, DARK_GREEN);
    Shell shellw(162, 0, 320, 200, DARK_RED);
    Shell shellm(158, 0, 162, 200, WHITE);
    
    shellb.print("Red on green\nIsn't this great.", DARK_RED);
    shellw.print("Oh the green and red of mayo\nI can see it still\nIts soft and craggy boglands\n"
        "Its tall majestic hills\nWhen the ocean kisses Ireland\nAnd the waves caress the shore\nOh the feeling it came over me\n"
        "to stay forevermore,\nFOREVERMORE", DARK_GREEN);
        
        
    while (1) {
        asm volatile("hlt");
    }
}
