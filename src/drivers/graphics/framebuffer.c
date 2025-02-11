#include "framebuffer.h"

/* draw pixel */
void draw_pixel(uint32_t *fb, size_t pitch, size_t x, size_t y, uint32_t color) {
    fb[y * pitch + x] = color;
}

/* clear screen */
void clear_screen(uint32_t *fb, size_t pitch, size_t width, size_t height, uint32_t color) {
    /* draw black pixels through entire framebuffer */
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            draw_pixel(fb, pitch, x, y, color);
        }
    }
}