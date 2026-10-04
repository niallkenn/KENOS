#include "terminal.h"
#include "multiboot.h"
#include "font.h"
#include "stdlib.h"
#include "unistd.h"

static int cursor_x = 0;
static int cursor_y = 0;

void terminal_put_char(char c, uint32_t fg, uint32_t bg) {
    if ((uint32_t)cursor_y >= mb_info->framebuffer_height) {
        cursor_x = 0;
        cursor_y = 0;
        terminal_clear(bg);
    }

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
        return;
    }

    if (c == '\t') {
        terminal_put_char(' ', fg, bg);
        terminal_put_char(' ', fg, bg);
        return;
    }

    if ((uint32_t)(cursor_x + FONT_WIDTH) > mb_info->framebuffer_width) {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
    }

    terminal_draw_char(c, cursor_x, cursor_y, fg, bg);
    cursor_x += FONT_WIDTH;
}

void terminal_draw_char(char c, int x, int y, uint32_t fg, uint32_t bg) {
    if (x < 0 || (uint32_t)(x + FONT_WIDTH) > mb_info->framebuffer_width ||
    y < 0 || (uint32_t)(y + FONT_HEIGHT) > mb_info->framebuffer_height) return;

    uint32_t* fb = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;
    uint32_t pitch = mb_info->framebuffer_pitch / 4;

    const uint8_t* glyph = font8x16[(uint8_t)c];

    for (int cy = 0; cy < FONT_HEIGHT; cy++) {
        uint8_t row = glyph[cy];
        for (int cx = 0; cx < FONT_WIDTH; cx++) {
            if (row & (1 << (7 - cx))) {
                fb[(y + cy) * pitch + (x + cx)] = fg;
            } else {
                fb[(y + cy) * pitch + (x + cx)] = bg;
            }
        }
    }
}


void terminal_print(const char* str, uint32_t fg, uint32_t bg) {
    int i = 0;
    while (str[i] != '\0') {
        terminal_put_char(str[i++], fg, bg);
    }
}

void terminal_clear(uint32_t bg) {
    uint32_t* fb = (uint32_t*)(uintptr_t)mb_info->framebuffer_addr;
    uint32_t pitch = mb_info->framebuffer_pitch / 4;

    for (uint32_t y = 0; y < mb_info->framebuffer_height; y++) {
        for (uint32_t x = 0; x < mb_info->framebuffer_width; x++) {
            fb[y * pitch + x] = bg;
        }
    }
}

void terminal_backspace(void) {
    if (cursor_x == 0  && cursor_y == 0) return;

    if (cursor_x == 0) {
        cursor_y -= FONT_HEIGHT;
        cursor_x = FONT_WIDTH * (mb_info->framebuffer_width / FONT_WIDTH - 1);
    } else cursor_x -= FONT_WIDTH;

    terminal_put_char(' ', WHITE, BLACK); // this should call write as it comes from ring3
    cursor_x -= FONT_WIDTH;
}

void terminal_main(void) {
    while (1) {
        int c = getchar();
        char ch = (char)c;
        write(1, &ch, 1);
    }
}