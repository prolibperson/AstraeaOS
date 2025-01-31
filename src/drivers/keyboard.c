#include "keyboard.h"
#include "terminal.h"
#include "idt.h"
#include "port.h"

#define KEYBOARD_PORT 0x60
#define KEYBOARD_CMD_PORT 0x64
#define INTERRUPT_ACK 0x20

#define KEY_ALT_1 0x01

#define KEY_BUFFER_SIZE 256
static char key_buffer[KEY_BUFFER_SIZE];
static size_t key_buffer_pos = 0;

static bool shift_pressed = false;

extern void keyboard_handler_stub();

static const char scancode_to_char[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // Backspace
    '\t', // Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // Enter
    0,  // Left Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,  // Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // Right Shift
    '*', 0, // Alt
    ' ', // Space
    0,  // Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
    0,  // Num Lock
    0,  // Scroll Lock
    0,  // Home key
    0x48,  // Up Arrow
    0,  // Page Up
    '-',
    0x4B,  // Left Arrow
    0,  // Center
    0x4D,  // Right Arrow
    '+',
    0,  // End key
    0x50,  // Down Arrow
    0,  // Page Down
    0,  // Insert key
    0,  // Delete key
    0, 0, 0, 0, 0, 0, 0, // F11-F12
    0  // Reserved keys
};

static const char scancode_to_char_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', // Backspace
    '\t', // Tab
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', // Enter
    0,  // Left Control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,  // Left Shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, // Right Shift
    '*', 0, // Alt
    ' ', // Space
    0,  // Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
    0,  // Num Lock
    0,  // Scroll Lock
    0,  // Home key
    0x48,  // Up Arrow
    0,  // Page Up
    '-',
    0x4B,  // Left Arrow
    0,  // Center
    0x4D,  // Right Arrow
    '+',
    0,  // End key
    0x50,  // Down Arrow
    0,  // Page Down
    0,  // Insert key
    0,  // Delete key
    0, 0, 0, 0, 0, 0, 0, // F11-F12
    0  // Reserved keys
};

void keyboard_handler_c(void) {
    if (inb(0x64) & 0x01) {
        uint8_t scancode = inb(KEYBOARD_PORT);

        if (scancode & 0x80) {
            scancode &= 0x7F;
            if (scancode == 0x2A || scancode == 0x36) {
                shift_pressed = false;
            }
            outb(0x20, INTERRUPT_ACK);
            return;
        }

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = true;
            outb(0x20, INTERRUPT_ACK);
            return;
        }

        char key_char = shift_pressed ? scancode_to_char_shift[scancode]
                                      : scancode_to_char[scancode];
        if (key_char) {
            if (scancode == '\b') {
                if (key_buffer_pos > 0) {
                    key_buffer_pos--;
                    terminal_putchar('\b');
                }
            } else if (key_buffer_pos < KEY_BUFFER_SIZE - 1) {
                key_buffer[key_buffer_pos++] = key_char;
            }
        }
    }
    outb(0x20, INTERRUPT_ACK);
}

char terminal_getchar(void) {
    while (key_buffer_pos == 0) {
        asm volatile ("hlt");
    }
    char c = key_buffer[0];
    for (size_t i = 1; i < key_buffer_pos; i++) {
        key_buffer[i - 1] = key_buffer[i];
    }
    key_buffer_pos--;
    return c;
}

void keyboard_init(void) {
    uint8_t status = inb(KEYBOARD_CMD_PORT);
    terminal_printf(PRINT_DEBUG, "Keyboard controller status: 0x%x\n", status);

    idt_set_entry(33, (uint64_t)keyboard_handler_stub, 0x08, 0x8E);
}