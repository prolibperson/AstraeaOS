#include "port.h"
#include "terminal.h"
#include "idt.h"

extern void timer_handler_stub();

static int tick_count = 0;

#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0   0x00
#define IDT_FLAG_INT_GATE 0x0E

void timer_handler() {
    tick_count++;
    if (tick_count >= 9) { // Approximately 500ms (assuming 18.2Hz PIT)
        terminal_toggle_cursor();
        tick_count = 0;
    }

    // Send EOI (End of Interrupt) to PIC
    outb(0x20, 0x20);
}

/* Initialize the PIT to a frequency of 18.2Hz */
void timer_init() {
    uint16_t divisor = 65536 / 18;  // 1.193182 MHz / 18.2Hz = 65536 / 18

    /* timer */
    idt_set_entry(32, (uint64_t)timer_handler_stub, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INT_GATE);

    outb(0x43, 0x36);             // Command port: Channel 0, Mode 3, Binary
    outb(0x40, divisor & 0xFF);   // Low byte
    outb(0x40, divisor >> 8);     // High byte
}
