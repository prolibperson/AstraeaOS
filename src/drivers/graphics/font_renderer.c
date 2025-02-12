#include "font_renderer.h"
#include "font.h"
#include "std.h"

/* draw char */
void draw_char(uint32_t *fb, size_t pitch, size_t x, size_t y, char c, uint32_t color) {
    if ((unsigned char)c > 127) return;

    size_t font_height = FONT_HEIGHT;
    const uint8_t *glyph = &AVGA2_8x14[(unsigned char)c * font_height];

    /* iterate through font array */
    for (size_t row = 0; row < font_height; row++) {
        for (size_t col = 0; col < 8; col++) {
            if (c == ' ' || !(glyph[row] & (1 << (7 - col)))) {
                /* if its a space or the pixel isn't set draw black */
                fb[(y + row) * pitch + (x + col)] = 0x000000;  
            } else {
                /* else draw the char normally */
                fb[(y + row) * pitch + (x + col)] = color;
            }
        }
    }
}


/* draw string */
void draw_string(uint32_t *fb, size_t pitch, size_t x, size_t y, const char *str, uint32_t color) {
    /* draw each char in the string */
    while (*str) {
        draw_char(fb, pitch, x, y, *str, color);
        x += 8;
        str++;
    }
}
