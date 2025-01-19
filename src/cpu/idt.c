#include "idt.h"
#include "port.h"
#include "terminal.h"

struct idt_entry idt[256];
struct idt_ptr idt_descriptor;

extern void idt_load(void);
extern void exception_wrappers(void);

#define WRAPPER_SIZE 10

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

void exception_handler(int num) {
    terminal_printf(PRINT_ERROR, "Exception: %s (IDT %d)\n", exception_messages[num], num);
    while (1) asm volatile ("hlt");
}

void register_exception_handlers() {
    for (int i = 0; i < 32; i++) {
        idt_set_entry(i, (uint32_t)(exception_wrappers + i * WRAPPER_SIZE), 0x08, 0x8E);
    }
}

void dummy_handler(int irq) {
    terminal_printf(PRINT_DEBUG, "IRQ %d received.\n", irq);
    outb(0x20, 0x20);
    if (irq >= 8) {
        outb(0xA0, 0x20);
    }
}

void pic_remap() {
    outb(0x20, 0x11); // Master PIC - ICW1
    outb(0xA0, 0x11); // Slave PIC - ICW1
    outb(0x21, 0x20); // Master PIC - ICW2 (offset 0x20)
    outb(0xA1, 0x28); // Slave PIC - ICW2 (offset 0x28)
    outb(0x21, 0x04); // Master PIC - ICW3 (IRQ2 -> Slave)
    outb(0xA1, 0x02); // Slave PIC - ICW3 (cascade identity)
    outb(0x21, 0x01); // Master PIC - ICW4
    outb(0xA1, 0x01); // Slave PIC - ICW4

    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}


bool idt_is_set(int num) {
    return idt[num].base_low != 0 || idt[num].base_high != 0 || idt[num].flags != 0;
}

void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    if (num < 0 || num >= 256) {
        terminal_printf(PRINT_ERROR, "Invalid IDT entry index: %d\n", num);
        return;
    }

    //terminal_printf(PRINT_DEBUG, "Setting IDT entry %d: base=0x%x, sel=0x%x, flags=0x%x\n", num, base, sel, flags);
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0   0x00
#define IDT_FLAG_INT_GATE 0x0E

void idt_init(void) {
    idt_descriptor.limit = sizeof(idt) - 1;
    idt_descriptor.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) {
        idt_set_entry(i, 0, 0, 0);
    }

    for (int i = 32; i < 48; i++) {
        if(i != 33)
            idt_set_entry(i, (uint32_t)dummy_handler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INT_GATE);
    }

    register_exception_handlers();

    idt_load();
}
