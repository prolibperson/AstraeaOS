#include "keyboard.h"
#include "terminal.h"
#include "idt.h"
#include "port.h"

#define KEYBOARD_PORT 0x60
#define KEYBOARD_CMD_PORT 0x64
#define INTERRUPT_ACK 0x20

extern void keyboard_handler_stub();

// US QWERTY keyboard scancode to ASCII map
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
    0,  // Up Arrow
    0,  // Page Up
    '-',
    0,  // Left Arrow
    0,  // Center (unused)
    0,  // Right Arrow
    '+',
    0,  // End key
    0,  // Down Arrow
    0,  // Page Down
    0,  // Insert key
    0,  // Delete key
    0, 0, 0, 0, 0, 0, 0, // F11-F12
    0  // Reserved keys
};

void keyboard_handler_c(void) {
    if (inb(0x64) & 0x01) {
        uint8_t scancode = inb(KEYBOARD_PORT);

        if (!(scancode & 0x80)) {
            char key_char = scancode_to_char[scancode];
            if (key_char) {
                terminal_putchar(key_char);
            } else {
                //terminal_printf(PRINT_ERROR, "Unknown scancode: 0x%x\n", scancode);
            }
        }
    }

    outb(0x20, 0x20);
}


void keyboard_init(void) {
    uint8_t status = inb(KEYBOARD_CMD_PORT);
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_DEBUG, "Keyboard controller status: 0x%x\n", status);
#endif

    idt_set_entry(33, (uint32_t)keyboard_handler_stub, 0x08, 0x8E);
}
