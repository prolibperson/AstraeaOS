#include "string.h"

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