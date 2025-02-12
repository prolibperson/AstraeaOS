#include "std.h"
#include "limine_manager.h"
#include "framebuffer.h"
#include "font_renderer.h"
#include "cpu_manager.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"

uint32_t *fb_ptr = 0;
size_t width = 0;
size_t height = 0;
size_t pitch = 0;

/* quick little graphics test i cooked up */
void color_rows() {
    /* rows */
    size_t num_rows = 20;
    size_t row_height = height / num_rows;
    uint32_t colors[num_rows];

    /* init colors */
    for (size_t i = 0; i < num_rows; i++) {
        uint8_t red = (i * 123) % 256;
        uint8_t green = (i * 231) % 256;
        uint8_t blue = (i * 97) % 256;
        colors[i] = (red << 16) | (green << 8) | blue;
    }

    /* rendering loop */
    size_t offset = 0;
    while (true) {
        /* draw color rows */
        for (size_t y = 0; y < height; y++) {
            size_t color_index = ((y / row_height) + offset) % num_rows;
            uint32_t color = colors[color_index];
            for (size_t x = 0; x < width; x++) {
                fb_ptr[y * pitch + x] = color;
            }
        }
        offset = (offset + 1) % num_rows;


        /* delay */
        for (volatile uint64_t delay = 0; delay < 10000000; delay++);
    }
}

void kmain(void) {
    /* init limine */
    limine_init();

    /* get framebuffer from limine request */
    struct limine_framebuffer *framebuffer = get_framebuffer();

    /* framebuffer info */
    fb_ptr = framebuffer->address;
    width = framebuffer->width;
    height = framebuffer->height;
    pitch = framebuffer->pitch / 4;

    /* initialize terminal */
    terminal_initialize(fb_ptr, pitch, height, width);
    //terminal_printf("Please note the shell is like nonexistent because im in the middle of a rewrite...\n");
    terminal_printf("Welcome to prolibOS!\n");

#ifdef DEBUG_BUILD
    tprintf("Debug Build preprocessor defined!\n");
#endif
    
    /* interrupts and keyboard driver init */
    gdt_init();
    idt_init();
    pic_remap();
    keyboard_init();
    timer_init();
    asm volatile ("sti"); /* enables interrupts */

    shell_run();

    /* halt cpu or else we exit the entrypoint and just return to bios */
    halt();
}