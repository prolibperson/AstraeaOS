#pragma once
#include "std.h"

#define tprintf terminal_printf

void terminal_initialize(uint32_t *fb, size_t fb_pitch, size_t height, size_t width);
void terminal_clear();
void terminal_scroll();
void terminal_newline();
void terminal_putchar(char c);
void terminal_printf(const char *fmt, ...);
void terminal_draw_cursor();
void terminal_toggle_cursor();
void terminal_setcolor(uint32_t color);
void terminal_setcolor_gradient(size_t line);