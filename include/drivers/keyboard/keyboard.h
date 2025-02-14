#pragma once

#include "std.h"

#define KEY_BUFFER_SIZE 256
extern char key_buffer[KEY_BUFFER_SIZE];
extern size_t key_buffer_pos;

void keyboard_init(void);
char terminal_getchar(void);

