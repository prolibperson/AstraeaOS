#pragma once

#include <stddef.h>

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);
void* memmove(void* dest, const void* src, size_t n);