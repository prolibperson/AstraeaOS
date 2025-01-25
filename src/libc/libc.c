#include "libc.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return original_dest;
}

char* strcat(char* dest, const char* src) {
    char* original_dest = dest;
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return original_dest;
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

void* memcpy(void* dest, const void* src, size_t num) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    while (num--) {
        *d++ = *s++;
    }
    return dest;
}

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

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s || n == 0) {
        return dest;
    }

    if (d < s) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }

    if (c == '\0') {
        return (char*)str;
    }

    return NULL;
}

static char* strtok_save_ptr = NULL;

char* strtok(char* str, const char* delimiters) {
    char* start;
    char* end;

    if (str != NULL) {
        strtok_save_ptr = str;
    }

    if (strtok_save_ptr == NULL) {
        return NULL;
    }

    start = strtok_save_ptr;
    while (*start && strchr(delimiters, *start)) {
        start++;
    }

    if (*start == '\0') {
        strtok_save_ptr = NULL;
        return NULL;
    }

    end = start;
    while (*end && !strchr(delimiters, *end)) {
        end++;
    }

    if (*end) {
        *end = '\0';
        strtok_save_ptr = end + 1;
    } else {
        strtok_save_ptr = NULL;
    }

    return start;
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