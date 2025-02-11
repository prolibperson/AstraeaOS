#include "gdt.h"

struct gdt_entry gdt[3];
struct gdt_ptr gdt_descriptor;

extern void gdt_load(void);

/* fill struct blah blah nobody cares */
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].access = access;
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= (granularity & 0xF0);
    gdt[num].base_high = (base >> 24) & 0xFF;
}

/* init */
void gdt_init(void) {
    /* set entries */
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xAF);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);

    /* set limit of gdt to the size of the array */
    gdt_descriptor.limit = sizeof(gdt) - 1;
    /* pointer to beginning of the gdt */
    gdt_descriptor.base = (uint64_t)&gdt;

    /* gdt assembly stub */
    gdt_load();
}