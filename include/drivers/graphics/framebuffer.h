#pragma once

#include "std.h"

void draw_pixel(uint32_t *fb, size_t pitch, size_t x, size_t y, uint32_t color);
void clear_screen(uint32_t *fb, size_t pitch, size_t width, size_t height, uint32_t color);