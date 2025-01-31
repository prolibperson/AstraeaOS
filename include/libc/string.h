#pragma once

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
char* strchr(const char* str, int c);
char* strtok(char* str, const char* delimiters);