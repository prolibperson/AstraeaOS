#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "keyboard.h"
#include "port.h"
#include "libc.h"
#include "shell.h"

/* kernel entrypoint */
void kernel_main(uint32_t multiboot_info_addr) {
    /* init terminal */
    terminal_initialize();

    /* welcome home sanitarium */
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_WELCOME, "- Welcome to prolibOS x86_64 v1.3.0! (BETA BUILD) -\n");
#else
    terminal_printf(PRINT_WELCOME, "- Welcome to prolibOS x86_64 v1.3.0! (RELEASE BUILD) -\n");
#endif
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

    /* (debug) check pic masks */
    uint8_t master_mask = inb(0x21);
    uint8_t slave_mask = inb(0xA1);
    terminal_printf(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);

    /* remap pic */
    pic_remap();
    terminal_printf(PRINT_INIT, "PIC Remapped\n");

    /* (debug) check pic masks again */
    master_mask = inb(0x21);
    slave_mask = inb(0xA1);
    terminal_printf(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);

    /* enable interrupts */
    terminal_printf(PRINT_INIT, "Enabling interrupts...\n");
    outb(0x21, inb(0x21) & ~0x02);
    asm volatile ("sti");
    terminal_printf(PRINT_INIT, "Interrupts enabled\n");
    outb(0x21, inb(0x21) & ~0x02);
    terminal_printf(PRINT_INIT, "Keyboard interrupts unmasked\n");

    /* hallo */
    terminal_printf(PRINT_INIT, "Starting shell\n");

    /* start shell */
    shell_run();

    /* halt cpu if shell isnt running */
    while (1) {
        asm volatile ("hlt");
    }
}