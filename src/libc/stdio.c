#include "stdio.h"
#include "string.h"

void itoa(int value, char* buffer, int base) {
    char* digits = "0123456789ABCDEF";
    char temp[32];
    int i = 0, j = 0, is_negative = 0;

    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }

    do {
        temp[i++] = digits[value % base];
        value /= base;
    } while (value > 0);

    if (is_negative) {
        temp[i++] = '-';
    }

    while (i--) {
        buffer[j++] = temp[i];
    }
    buffer[j] = '\0';
}

int vsnprintf(char* buffer, size_t size, const char* format, va_list args) {
    char* out = buffer;
    const char* end = buffer + size - 1;
    const char* ptr = format;
    char temp[32];

    while (*ptr) {
        if (*ptr == '%') {
            ptr++;
            switch (*ptr) {
                case 'd': {
                    int value = va_arg(args, int);
                    itoa(value, temp, 10);
                    for (char* t = temp; *t && out < end; t++) {
                        *out++ = *t;
                    }
                    break;
                }
                case 'x': {
                    int value = va_arg(args, int);
                    itoa(value, temp, 16);
                    for (char* t = temp; *t && out < end; t++) {
                        *out++ = *t;
                    }
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    while (*str && out < end) {
                        *out++ = *str++;
                    }
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    if (out < end) {
                        *out++ = c;
                    }
                    break;
                }
                case '%': {
                    if (out < end) {
                        *out++ = '%';
                    }
                    break;
                }
                default: {
                    if (out < end) {
                        *out++ = '%';
                    }
                    if (out < end) {
                        *out++ = *ptr;
                    }
                    break;
                }
            }
        } else {
            if (out < end) {
                *out++ = *ptr;
            }
        }
        ptr++;
    }

    if (size > 0) {
        *out = '\0';
    }

    return (int)(out - buffer);
}