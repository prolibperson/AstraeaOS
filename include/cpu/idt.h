#ifndef IDT_H
#define IDT_H

#include "global.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_init(void);
void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags);
void pic_remap();
bool check_interrupt_conditions();
bool idt_is_set(int num);

#endif // IDT_H
