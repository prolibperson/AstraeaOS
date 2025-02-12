#include "idt.h"
#include "port.h"
#include "terminal.h"
#include "timer.h"

struct idt_entry idt[256];
struct idt_ptr idt_descriptor;

extern void idt_load(void);
extern void exception_wrappers(void);

#define WRAPPER_SIZE 16

/* messages */
const char *exception_messages[] = {
    "Division by Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* exception handler */
void exception_handler(int num, uint64_t error_code) {
    tprintf("\nKernel Panic!\n");
    tprintf("Exception: %s (IDT %d, Error Code: 0x%lx)\n", exception_messages[num], num, error_code);
    while (1) asm volatile ("hlt");
}

/* register the handlers */
void register_exception_handlers() {
    for (int i = 0; i < 32; i++) {
        idt_set_entry(i, (uint64_t)(exception_wrappers + i * WRAPPER_SIZE), 0x08, 0x8E);
    }
}

/* dummy handler */
void dummy_handler(int irq) {
    tprintf("IRQ %d received.\n", irq);
    outb(0x20, 0x20);
    if (irq >= 8) {
        outb(0xA0, 0x20);
    }
}

/* remap pic */
void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

/* check if the idt is set */
bool idt_is_set(int num) {
    return idt[num].base_low != 0 || idt[num].base_high != 0 || idt[num].flags != 0;
}

/* fill up struct no one cares boring i hate explaining my own code */
void idt_set_entry(int num, uint64_t base, uint16_t sel, uint8_t flags) {
    if (num < 0 || num >= 256) {
        tprintf("Invalid IDT entry index: %d\n", num);
        return;
    }

    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].base_upper = (base >> 32) & 0xFFFFFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].reserved = 0;
}

#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0   0x00
#define IDT_FLAG_INT_GATE 0x0E

/* idt init */
void idt_init(void) {
    idt_descriptor.limit = sizeof(idt) - 1;
    idt_descriptor.base = (uint64_t)&idt;

    /* fill entries */
    for (int i = 0; i < 256; i++) {
        idt_set_entry(i, 0, 0, 0);
    }

    /* register exception handlers */
    register_exception_handlers();

    /* register dummy handlers */
    for (int i = 32; i < 48; i++) {
        /* if not keyboard interrupt gate */
        if (i != 33)
            idt_set_entry(i, (uint64_t)dummy_handler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INT_GATE);
    }

    /* remap pic */
    pic_remap();

    /* assembly stub */
    idt_load();
}