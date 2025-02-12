#include "shell.h"
#include "terminal.h"
#include "libc.h"
#include "keyboard.h"

#define HISTORY_SIZE 10

char* command_arguments = NULL;
static const shell_command_t commands[] = {
    { "help", "List all available commands", shell_help },
    { "echo", "Print text to the terminal",  shell_echo },
    { "clear","Clear the terminal",          shell_clear },
    { "reboot","Reboot the system",         shell_reboot },
};
static const size_t command_count = sizeof(commands) / sizeof(commands[0]);

static char history[HISTORY_SIZE][256];
static int history_count = 0;
static int history_index = -1;

void shell_reboot(void) {
    tprintf("Rebooting system...\n");
    asm volatile (
        "cli\n"
        "movb $0xFE, %al\n" 
        "outb %al, $0x64\n"
        "hlt\n"
    );
}

void shell_clear(void) {
    terminal_clear(); 
}

void shell_help(void) {
    tprintf("Available commands:\n");
    for (size_t i = 0; i < command_count; i++) {
        tprintf(" - ");
        tprintf(commands[i].name);
        tprintf(": ");
        tprintf(commands[i].description);
        terminal_putchar('\n');
    }
}

void shell_echo(void) {
    if (command_arguments && *command_arguments) {
        tprintf(command_arguments);
    } else {
        tprintf("\n");
    }
    terminal_putchar('\n');
}

void clear_input_field(size_t input_len) {
    for (size_t i = 0; i < input_len; i++) {
        terminal_putchar('\b');
        terminal_putchar(' ');
        terminal_putchar('\b');
    }
    //for (size_t i = 0; i < input_len; i++) {
        //terminal_putchar(' ');
    //}
    //for (size_t i = 0; i < input_len; i++) {
        //terminal_putchar('\b');
    //}
}

/* check for if the input is only spaces */
int is_input_only_spaces(const char* input, size_t input_len) {
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

/* main shell function (yes i know its a super function bruh) */
void shell_run(void) {
    char input[256];
    size_t input_len = 0;

    /* shell loop */
    while (1) {
        /* print the thing */
        tprintf("\nuser@prolibOS $ ");

        input_len = 0;
        history_index = -1;

        char c;
        while ((c = terminal_getchar()) != '\n') {
            if (c == '\b') { // backspace
                if (input_len > 0) {
                    input_len--;
                    terminal_putchar('\b');
                }
            } else if (c == 0x48) { // up arrow
                if (history_count > 0) {
                    if (history_index == -1) {
                        history_index = 0;
                    } else if (history_index < history_count - 1) {
                        history_index++;
                    }
                    strcpy(input, history[history_count - 1 - history_index]);
                    input_len = strlen(input);

                    clear_input_field(input_len);
                    tprintf("\ruser@prolibOS $ ");
                    tprintf(input);
                }
            } else if (c == 0x50) { // down arrow
                if (history_index > 0) {
                    history_index--;
                    strcpy(input, history[history_count - 1 - history_index]);
                    input_len = strlen(input);

                    clear_input_field(input_len);
                    tprintf("\ruser@prolibOS $ ");
                    tprintf(input);
                } else if (history_index == 0) {
                    history_index = -1;
                    input[0] = '\0';

                    clear_input_field(input_len);
                    input_len = 0;
                    tprintf("\ruser@prolibOS $ ");
                }
            } else if (input_len < sizeof(input) - 1) {
                input[input_len++] = c;
                terminal_putchar(c);
            }
        }
        if (input_len == 0 || is_input_only_spaces(input, input_len)) {
            tprintf("\n");
            continue;
        }
        input[input_len] = '\0';

        if (input_len > 0 && (history_count == 0 || strcmp(history[history_count - 1], input) != 0)) {
            if (history_count < HISTORY_SIZE) {
                strcpy(history[history_count], input);
                history_count++;
            } else {
                for (int i = 1; i < HISTORY_SIZE; i++) {
                    strcpy(history[i - 1], history[i]);
                }
                strcpy(history[HISTORY_SIZE - 1], input);
            }
        }

        char* command_name = strtok(input, " ");
        command_arguments = strtok(NULL, "");

        int found = 0;
        for (size_t i = 0; i < command_count; i++) {
            if (strcmp(command_name, commands[i].name) == 0) {
                terminal_putchar('\n');
                commands[i].function();
                found = 1;
                break;
            }
        }
        if (!found) {
            terminal_putchar('\n');
            tprintf("Unknown command: ");
            tprintf(command_name);
            terminal_putchar('\n');
        }
    }
}
