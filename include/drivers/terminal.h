#pragma once
#include "global.h"

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

enum print_type {
	PRINT_INIT = 0,
	PRINT_DEBUG = 1,
	PRINT_ERROR = 2,
	PRINT_WELCOME = 3,
	PRINT_LOGGING = 4,
};

void terminal_initialize(void);
void terminal_clear();
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_printf(uint8_t print_type, const char* format, ...);
void terminal_clear_line(size_t row);
void update_cursor(size_t row, size_t column);
void terminal_print_centered_rainbow(const char* text);