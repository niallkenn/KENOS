#include "terminal.h"
#include "multiboot.h"
#include "font.h"
#include "stdlib.h"

static int cursor_x = 0;
static int cursor_y = 0;

void put_char(char c, uint32_t fg, uint32_t bg) {
    uint32_t* fb = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;
    uint32_t pitch = mb_info->framebuffer_pitch / 4;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
    } else {
        const uint8_t* glyph = font8x16[(uint8_t)c];

        for (int cy = 0; cy < FONT_HEIGHT; cy++) {
            uint8_t row = glyph[cy];
            for (int cx = 0; cx < FONT_WIDTH; cx++) {
                if (row & (1 << (7 - cx))) {
                    fb[(cursor_y + cy) * pitch + (cursor_x + cx)] = fg;
                } else {
                    fb[(cursor_y + cy) * pitch + (cursor_x + cx)] = bg;
                }
            }
        }

        cursor_x += FONT_WIDTH;
        if ((uint32_t)(cursor_x + FONT_WIDTH) > mb_info->framebuffer_width) {
            cursor_x = 0;
            cursor_y += FONT_HEIGHT;
        }
    }

    if ((uint32_t)(cursor_y + FONT_HEIGHT) > mb_info->framebuffer_height) {
        cursor_x = 0;
        cursor_y = 0;
        clear(bg);
    }
}

void print(const char* str, uint32_t fg, uint32_t bg) {
    int i = 0;
    while (str[i] != '\0') {
        put_char(str[i++], fg, bg);
    }
}

void clear(uint32_t bg) {
    uint32_t* fb = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;
    uint32_t pitch = mb_info->framebuffer_pitch / 4;

    for (uint32_t y = 0; y < mb_info->framebuffer_height; y++) {
        for (uint32_t x = 0; x < mb_info->framebuffer_width; x++) {
            fb[y * pitch + x] = bg;
        }
    }
}
