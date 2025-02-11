#pragma once

#include "std.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void idt_init(void);
void idt_set_entry(int num, uint64_t base, uint16_t sel, uint8_t flags);
void pic_remap();
bool idt_is_set(int num);