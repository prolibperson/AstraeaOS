#pragma once

#include "std.h"

#define FONT_HEIGHT 14

void draw_char(uint32_t *fb, size_t pitch, size_t x, size_t y, char c, uint32_t color);
void draw_string(uint32_t *fb, size_t pitch, size_t x, size_t y, const char *str, uint32_t color);