#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "keyboard.h"
#include "port.h"
#include "libc.h"
#include "shell.h"
#include "vga.h"

/* kernel entrypoint */
void kernel_main(uint32_t multiboot_info_addr) {
    /* init terminal */
    terminal_initialize();

#ifdef DEBUG_BUILD
    terminal_printf(PRINT_DEBUG, "Beta Build!\n");
#endif

#ifdef DEBUG_BUILD
    terminal_printf(PRINT_INIT, "Kernel is starting...\n");
#endif

    /* initialize global descriptor table */
    gdt_init();
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_LOGGING, "GDT Initialized\n");
    #endif

    /* initialize interrupt descriptor table */
    idt_init();
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_LOGGING, "IDT Initialized\n");
#endif

    /* initialize keyboard driver / handler */
    keyboard_init();
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_LOGGING, "Keyboard initialized\n");
#endif

    /* (debug) check pic masks */
    uint8_t master_mask = inb(0x21);
    uint8_t slave_mask = inb(0xA1);
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);
#endif

    /* remap pic */
    pic_remap();
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_LOGGING, "PIC Remapped\n");
#endif

    /* (debug) check pic masks again */
    master_mask = inb(0x21);
    slave_mask = inb(0xA1);
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);
#endif

    /* enable interrupts */
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_INIT, "Enabling interrupts...\n");
#endif
    outb(0x21, inb(0x21) & ~0x02);
    asm volatile ("sti");
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_LOGGING, "Interrupts enabled\n");
#endif
    outb(0x21, inb(0x21) & ~0x02);
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_LOGGING, "Keyboard interrupts unmasked\n");
#endif

    /* hallo */
#ifdef DEBUG_BUILD
    terminal_printf(PRINT_INIT, "Starting shell\n");
#endif

    /* welcome home sanitarium (yes i know im lazy) */
    terminal_clear();
    terminal_writestring("                                  _ _ _        ____   _____ \n");
    terminal_writestring("                                 | (_) |      / __ \\ / ____|\n");
    terminal_writestring("                  _ __  _ __ ___ | |_| |__   | |  | | (___  \n");
    terminal_writestring("                 | '_ \\| '__/ _ \\| | | '_ \\  | |  | |\\___ \\ \n");
    terminal_writestring("                 | |_) | | | (_) | | | |_) | | |__| |____) | \n");
    terminal_writestring("                 | .__/|_|  \\___/|_|_|_.__/   \\____/|_____/  \n");
    terminal_writestring("                 | |                                         \n");
    terminal_writestring("                 |_|                                        \n\n");
    terminal_writestring("\n        Welcome to prolibOS! You can run 'help' for a list of commands.\n");

    /* start shell */
    shell_run();

    /* halt cpu if shell isnt running */
    while (1) {
        asm volatile ("hlt");
    }
}