#include "memory.h"

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

void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

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