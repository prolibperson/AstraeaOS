#pragma once

#include <stddef.h>

typedef void (*command_function_t)(void);

typedef struct {
    const char* name;
    const char* description;
    command_function_t function;
} shell_command_t;

void shell_run(void);
void shell_help(void);
void shell_echo(void);
void shell_clear(void);
void shell_reboot(void);
void shell_rng(void);
void shell_info(void);