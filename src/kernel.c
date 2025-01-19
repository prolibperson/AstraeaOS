#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "keyboard.h"
#include "port.h"

// kernel entrypoint
void kernel_main(void) {
    /* init terminal */
    terminal_initialize();

    /* welcome home sanitarium */
    terminal_printf(PRINT_WELCOME, "- Welcome to prolib OS! -\n");
    terminal_printf(PRINT_INIT, "Kernel is starting...\n");

    /* initialize global descriptor table */
    gdt_init();
    terminal_printf(PRINT_INIT, "GDT Initialized\n");

    /* initialize interrupt descriptor table */
    idt_init();
    terminal_printf(PRINT_INIT, "IDT Initialized\n");

    /* initialize keyboard driver / handler */
    keyboard_init();
    terminal_printf(PRINT_INIT, "Keyboard initialized\n");

#ifdef DEBUG_BUILD 
    /* (debug) check pic masks */
    uint8_t master_mask = inb(0x21);
    uint8_t slave_mask = inb(0xA1);
    terminal_printf(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);
#endif

    /* remap pic */
    pic_remap();
    terminal_printf(PRINT_INIT, "PIC Remapped\n");

#ifdef DEBUG_BUILD 
    /* (debug) check pic masks again */
    uint8_t master_mask2 = inb(0x21);
    uint8_t slave_mask2 = inb(0xA1);
    terminal_printf(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask2, slave_mask2);
#endif

    /* enable interrupts */
    terminal_printf(PRINT_INIT, "Enabling interrupts...\n");
    outb(0x21, inb(0x21) & ~0x02);
    asm volatile ("sti");
    terminal_printf(PRINT_INIT, "Interrupts enabled\n");
    outb(0x21, inb(0x21) & ~0x02);
    terminal_printf(PRINT_INIT, "Keyboard interrupts unmasked\n");

    /* (placeholder) print the shell thingy until */
    /* i implement an actual proper shell system  */
    terminal_writestring("root@prolibOS $ ");

    /* halt cpu */
    while (1) {
        asm volatile ("hlt");
    }
}
