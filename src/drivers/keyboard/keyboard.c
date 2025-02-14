#include "keyboard.h"
#include "terminal.h"
#include "idt.h"
#include "port.h"
#include "scancodes.h"

#define KEYBOARD_PORT 0x60
#define KEYBOARD_CMD_PORT 0x64
#define INTERRUPT_ACK 0x20

char key_buffer[KEY_BUFFER_SIZE];
size_t key_buffer_pos = 0;

static bool shift_pressed = false;

extern void keyboard_handler_stub();

void keyboard_handler_c(void) {
    /* read port */
    if (inb(0x64) & 0x01) {
        /* get scancode */
        uint8_t scancode = inb(KEYBOARD_PORT);

        /* check if shift is pressed */
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

        /* add key to keybuffer */
        if (key_char) {
            /* check if the key pressed is backspace */
            if (scancode == '\b') {
                /* go back in key buffer and remove the last character */
                if (key_buffer_pos > 0) {
                    key_buffer_pos--;
                    terminal_putchar('\b');
                }
            } 
            /* else, add the key to the keybuffer */
            else if (key_buffer_pos < KEY_BUFFER_SIZE - 1) {
                key_buffer[key_buffer_pos++] = key_char;
            }
        }
    }
    outb(0x20, INTERRUPT_ACK);
}

/* get character from the keybuffer */
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
    /* set keyboard entry */
    idt_set_entry(33, (uint64_t)keyboard_handler_stub, 0x08, 0x8E);
}