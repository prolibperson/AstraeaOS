#ifndef LIBC_H
#define LIBC_H

#include "global.h"

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);
void itoa(int value, char* buffer, int base);
void *memmove(void *dest, const void *src, size_t n);
char* strtok(char* str, const char* delimiters);
char* strchr(const char* str, int c);

#endif // LIBC_H
