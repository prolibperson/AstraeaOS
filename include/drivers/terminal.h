#ifndef TERMINAL_H
#define TERMINAL_H

#include "global.h"

#include "vga.h"

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

#endif // TERMINAL_H
