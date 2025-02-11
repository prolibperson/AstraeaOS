#include "keyboard.h"
#include "terminal.h"
#include "idt.h"
#include "port.h"
#include "scancodes.h"

#define KEYBOARD_PORT 0x60
#define KEYBOARD_CMD_PORT 0x64
#define INTERRUPT_ACK 0x20

static bool shift_pressed = false;

extern void keyboard_handler_stub();

void keyboard_handler_c(void) {
    /* read interrupt */
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

        /* (placeholder) print out the key pressed */             
        if (key_char) {
            if (scancode == '\b') {
                terminal_putchar('\b');
            } else {
                terminal_putchar(key_char);
            }
        }
    }
    /* send end of interrupt */
    outb(0x20, INTERRUPT_ACK);
}

void keyboard_init(void) {
    /* (debug) get status of keyboard port */
    //uint8_t status = inb(KEYBOARD_CMD_PORT);
    //tprintf("Keyboard controller status: 0x%x\n", status);

    /* set keyboard entry */
    idt_set_entry(33, (uint64_t)keyboard_handler_stub, 0x08, 0x8E);
}