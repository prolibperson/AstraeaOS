#include "shell.h"
#include "terminal.h"
#include "libc.h"
#include "keyboard.h"

char* command_arguments = NULL;

static const shell_command_t commands[] = {
    { "help", "List all available commands", shell_help },
    { "echo", "Print text to the terminal", shell_echo },
};

static const size_t command_count = sizeof(commands) / sizeof(commands[0]);

void shell_help(void) {
    terminal_writestring("Available commands:\n");
    for (size_t i = 0; i < command_count; i++) {
        terminal_writestring(" - ");
        terminal_writestring(commands[i].name);
        terminal_writestring(": ");
        terminal_writestring(commands[i].description);
        terminal_putchar('\n');
    }
}

void shell_echo(void) {
    if (command_arguments && *command_arguments) {
        terminal_writestring(command_arguments);
    } else {
        terminal_writestring("\n");
    }
    terminal_putchar('\n');
}

void shell_run(void) {
    char input[256];
    size_t input_len = 0;

    while (1) {
        terminal_writestring("\nuser@prolibOS $ ");
        input_len = 0;

        char c;
        while ((c = terminal_getchar()) != '\n') {
            if (input_len < sizeof(input) - 1) {
                input[input_len++] = c;
                terminal_putchar(c);
            }
        }
        input[input_len] = '\0';

        char* command_name = strtok(input, " ");
        command_arguments = strtok(NULL, "");

        int found = 0;
        for (size_t i = 0; i < command_count; i++) {
            if (strcmp(input, commands[i].name) == 0) {
                terminal_putchar('\n');
                commands[i].function();
                found = 1;
                break;
            }
        }
        if (!found) {
            terminal_putchar('\n');
            terminal_writestring("Unknown command: ");
            terminal_writestring(input);
            terminal_putchar('\n');
        }
    }
}
