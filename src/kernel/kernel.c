#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "keyboard.h"
#include "port.h"
#include "libc.h"
#include "shell.h"
#include "vga.h"
#include "multiboot2.h"

void halt_cpu() {
    while(1)
        asm volatile("hlt");
}

void init_idt_gdt() {
    gdt_init();
    idt_init();
    pic_remap();
}

void init_kernel_shell() {
    /* init terminal */
    terminal_initialize();

    tprintf("Starting Boot Process...\n\n");

#ifdef DEBUG_BUILD
    tprintfp(PRINT_DEBUG, "Beta Build!\n");
#endif

    tprintfp(PRINT_INIT, "Kernel is starting...\n");

    /* initialize global descriptor table */
    gdt_init();
    tprintfp(PRINT_LOGGING, "GDT Initialized\n");

    /* initialize interrupt descriptor table */
    idt_init();
    tprintfp(PRINT_LOGGING, "IDT Initialized\n");

    /* initialize keyboard driver / handler */
    keyboard_init();
    tprintfp(PRINT_LOGGING, "Keyboard initialized\n");

    /* (debug) check pic masks */
    uint8_t master_mask = inb(0x21);
    uint8_t slave_mask = inb(0xA1);
#ifdef DEBUG_BUILD
    tprintfp(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);
#endif

    /* remap pic */
    pic_remap();
    tprintfp(PRINT_LOGGING, "PIC Remapped\n");

    /* (debug) check pic masks again */
    master_mask = inb(0x21);
    slave_mask = inb(0xA1);
#ifdef DEBUG_BUILD
    tprintfp(PRINT_DEBUG, "PIC Masks: Master=0x%x, Slave=0x%x\n", master_mask, slave_mask);
#endif

    /* enable interrupts */
    tprintfp(PRINT_INIT, "Enabling interrupts...\n");
    outb(0x21, inb(0x21) & ~0x02);
    asm volatile ("sti");
    tprintfp(PRINT_LOGGING, "Interrupts enabled\n");
    outb(0x21, inb(0x21) & ~0x02);
    tprintfp(PRINT_LOGGING, "Keyboard interrupts unmasked\n");

    /* hallo */
    tprintfp(PRINT_INIT, "Starting shell\n");

    /* welcome home sanitarium (yes i know im lazy) */
    terminal_clear();
    tprintf("                                  _ _ _        ____   _____ \n");
    tprintf("                                 | (_) |      / __ \\ / ____|\n");
    tprintf("                  _ __  _ __ ___ | |_| |__   | |  | | (___  \n");
    tprintf("                 | '_ \\| '__/ _ \\| | | '_ \\  | |  | |\\___ \\ \n");
    tprintf("                 | |_) | | | (_) | | | |_) | | |__| |____) | \n");
    tprintf("                 | .__/|_|  \\___/|_|_|_.__/   \\____/|_____/  \n");
    tprintf("                 | |                                         \n");
    tprintf("                 |_|                                        \n\n");
    tprintf("\n        Welcome to prolibOS! You can run 'help' for a list of commands.\n");

    /* start shell */
    shell_run();

    /* halt cpu if shell isnt running */
    halt_cpu();
}

// Kernel main function
void kernel_main(uint64_t multiboot_info_addr) {
    init_kernel_shell();
    halt_cpu();
}

