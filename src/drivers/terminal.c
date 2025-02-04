#include "terminal.h"
#include "vga.h"
#include "libc.h"
#include "port.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void update_cursor(size_t row, size_t column) {
    uint16_t position = row * VGA_WIDTH + column;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    terminal_clear();
    update_cursor(terminal_row, terminal_column);
}

void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    update_cursor(terminal_row, terminal_column);
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll() {
    memmove(terminal_buffer, terminal_buffer + VGA_WIDTH, (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(uint16_t));

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }

    terminal_row = VGA_HEIGHT - 1;
    update_cursor(terminal_row, terminal_column);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\r') {
        terminal_column = 0;
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
        } else if (terminal_row > 0) {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
        }

        const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = vga_entry(' ', terminal_color);

        update_cursor(terminal_row, terminal_column);
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
            if (terminal_row == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }
    update_cursor(terminal_row, terminal_column);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++; // move past "%"
            switch (format[i]) {
                case 'c': { // character
                    char c = (char)va_arg(args, int);
                    terminal_putchar(c);
                    break;
                }
                case 's': { // string
                    const char* s = va_arg(args, const char*);
                    terminal_write(s, strlen(s));
                    break;
                }
                case 'd': { // decimal integer
                    int value = va_arg(args, int);
                    char buffer[12];
                    itoa(value, buffer, 10);
                    terminal_write(buffer, strlen(buffer));
                    break;
                }
                case 'x': { // hexadecimal integer
                    int value = va_arg(args, int);
                    char buffer[9];
                    itoa(value, buffer, 16);
                    terminal_write(buffer, strlen(buffer));
                    break;
                }
                default: { // unhandled specifier
                    terminal_putchar('%');
                    terminal_putchar(format[i]);
                    break;
                }
            }
        } else {
            terminal_putchar(format[i]);
        }
    }

    va_end(args);
}

void terminal_printf_prefix(uint8_t print_type, const char* format, ...) {
    va_list args;
    va_start(args, format);

    const int padding_width = 10;
    const char* prefix = NULL;

    switch (print_type) {
        case PRINT_INIT:
            terminal_setcolor(VGA_COLOR_GREEN);
            prefix = "[INIT]";
            break;
        case PRINT_DEBUG:
#ifndef  DEBUG_BUILD
                return;
#else
            terminal_setcolor(VGA_COLOR_CYAN);
            prefix = "[DEBUG]";
            break;
#endif
        case PRINT_ERROR:
            terminal_setcolor(VGA_COLOR_RED);
            prefix = "[ERROR]";
            break;
        case PRINT_WELCOME:
            prefix = NULL;
            break;
        case PRINT_LOGGING:
        default:
            terminal_setcolor(VGA_COLOR_WHITE);
            prefix = "[LOGGING]";
            break;
    }

    if (prefix) {
        char padded_prefix[padding_width + 1];
        size_t prefix_len = strlen(prefix);
        for (size_t i = 0; i < padding_width; i++) {
            if (i < prefix_len) {
                padded_prefix[i] = prefix[i];
            } else {
                padded_prefix[i] = ' ';
            }
        }
        padded_prefix[padding_width] = '\0';
        terminal_write(padded_prefix, padding_width);
    }

    if (print_type == PRINT_WELCOME) {
        const int terminal_width = 80;
        size_t message_len = strlen(format);
        int leading_spaces = (terminal_width - message_len) / 2;
        int trailing_dashes = terminal_width;

        for (int i = 0; i < leading_spaces; i++) {
            terminal_putchar(' ');
        }

        for (size_t i = 0; format[i] != '\0'; i++) {
            terminal_putchar(format[i]);
        }

        for (int i = 0; i < trailing_dashes; i++) {
            terminal_putchar('-');
        }

        va_end(args);
        return;
    }

    terminal_setcolor(VGA_COLOR_LIGHT_GREY);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++; // move past "%"
            switch (format[i]) {
                case 'c': { // character
                    char c = (char)va_arg(args, int);
                    terminal_putchar(c);
                    break;
                }
                case 's': { // string
                    const char* s = va_arg(args, const char*);
                    terminal_write(s, strlen(s));
                    break;
                }
                case 'd': { // decimal integer
                    int value = va_arg(args, int);
                    char buffer[12];
                    itoa(value, buffer, 10);
                    terminal_write(buffer, strlen(buffer));
                    break;
                }
                case 'x': { // hexadecimal integer
                    int value = va_arg(args, int);
                    char buffer[9];
                    itoa(value, buffer, 16);
                    terminal_write(buffer, strlen(buffer));
                    break;
                }
                default: { // unhandled specifier
                    terminal_putchar('%');
                    terminal_putchar(format[i]);
                    break;
                }
            }
        } else {
            terminal_putchar(format[i]);
        }
    }

    va_end(args);
}