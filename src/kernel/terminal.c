#include "std.h"
#include "font_renderer.h"
#include "framebuffer.h"

#define CHAR_WIDTH      8
#define CHAR_HEIGHT     FONT_HEIGHT
#define CURSOR_CHAR     '_'

static uint32_t *framebuffer;
static size_t pitch;
static size_t cursor_x = 0;
static size_t cursor_y = 0;
static size_t fb_width = 0;
static size_t fb_height = 0;
static uint32_t terminal_color = 0xFFFFFF;

/* determine terminal width and height based on framebuffer resolution */
#define TERMINAL_WIDTH  (fb_width / CHAR_WIDTH)
#define TERMINAL_HEIGHT (fb_height / CHAR_HEIGHT)

static int cursor_visible = 1;

/* reset cursor and fill framebuffer with black pixels */
void terminal_clear() {
    /* clear framebuffer */
    for (size_t y = 0; y < fb_height; y++) {
        for (size_t x = 0; x < fb_width; x++) {
            draw_pixel(framebuffer, pitch, x, y, 0x000000);
        }
    }

    cursor_x = 0;
    cursor_y = 0;
}


/* initialize all vars and clear terminal */
void terminal_initialize(uint32_t *fb, size_t fb_pitch, size_t height, size_t width) {
    framebuffer = fb;
    pitch = fb_pitch;
    fb_width = width;
    fb_height = height;
    cursor_x = 0;
    cursor_y = 0;

    terminal_clear();
}

/* draw cursor */
void terminal_draw_cursor() {
    if (cursor_visible) {
        
    } else {
        draw_char(framebuffer, pitch, cursor_x * CHAR_WIDTH, cursor_y * CHAR_HEIGHT, ' ', terminal_color);
    }
}

/* change bool for blinking */
void terminal_toggle_cursor() {
    cursor_visible = !cursor_visible;
    terminal_draw_cursor();
}

/* set the terminal text color */
void terminal_setcolor(uint32_t color) {
    terminal_color = color;
}

void terminal_setcolor_gradient(size_t line) {
    uint8_t red = (line * 15) % 256;
    uint8_t green = (line * 5) % 256;
    uint8_t blue = 255;
    terminal_color = (red << 16) | (green << 8) | blue;
}

/* scroll */
void terminal_scroll() {
    /* scroll everything up */
    for (size_t y = 0; y < (TERMINAL_HEIGHT - 1) * CHAR_HEIGHT; y++) {
        for (size_t x = 0; x < fb_width; x++) {
            framebuffer[y * pitch + x] = framebuffer[(y + CHAR_HEIGHT) * pitch + x];
        }
    }
    
    /* clear bottom part of the screen */
    for (size_t y = (TERMINAL_HEIGHT - 1) * CHAR_HEIGHT; y < fb_height; y++) {
        for (size_t x = 0; x < fb_width; x++) {
            framebuffer[y * pitch + x] = 0x000000;
        }
    }

    /* move cursor */
    if (cursor_y > 0) {
        cursor_y--;
    }
}

/* newline */
void terminal_newline() {
    cursor_x = 0;
    cursor_y++;
    if (cursor_y >= TERMINAL_HEIGHT) {
        terminal_scroll();
    }
}

/* put char */
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_newline();
        return;
    }
    if (c == '\r') {
        cursor_x = 0;
        return;
    }

    if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            draw_char(framebuffer, pitch, cursor_x * CHAR_WIDTH, cursor_y * CHAR_HEIGHT, ' ', 0x000000);
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = TERMINAL_WIDTH - 1;
            draw_char(framebuffer, pitch, cursor_x * CHAR_WIDTH, cursor_y * CHAR_HEIGHT, ' ', 0x000000);
        }
        return;
    }

    if (cursor_x >= TERMINAL_WIDTH || cursor_y >= TERMINAL_HEIGHT) {
        return;
    }

    draw_char(framebuffer, pitch, cursor_x * CHAR_WIDTH, cursor_y * CHAR_HEIGHT, c, terminal_color);

    cursor_x++;
    if (cursor_x >= TERMINAL_WIDTH) {
        terminal_newline();
    }
}

/* i stole the printf switch case from somewhere on stack overflow :P */
void terminal_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c': {
                    char c = (char) va_arg(args, int);
                    terminal_putchar(c);
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    while (*str) {
                        terminal_putchar(*str++);
                    }
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    int i = 0;
                    bool isNegative = false;

                    if (num < 0) {
                        isNegative = true;
                        num = -num;
                    }

                    do {
                        buffer[i++] = num % 10 + '0';
                        num /= 10;
                    } while (num > 0);

                    if (isNegative) {
                        buffer[i++] = '-';
                    }

                    while (i > 0) {
                        terminal_putchar(buffer[--i]);
                    }
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    char buffer[32];
                    int i = 0;

                    do {
                        int remainder = num % 16;
                        buffer[i++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'a';
                        num /= 16;
                    } while (num > 0);

                    while (i > 0) {
                        terminal_putchar(buffer[--i]);
                    }
                    break;
                }
                case 'f': {
                    double num = va_arg(args, double);
                    char buffer[32];
                    int i = 0;

                    if (num < 0) {
                        terminal_putchar('-');
                        num = -num;
                    }

                    int intPart = (int)num;
                    double fracPart = num - intPart;

                    do {
                        buffer[i++] = intPart % 10 + '0';
                        intPart /= 10;
                    } while (intPart > 0);

                    while (i > 0) {
                        terminal_putchar(buffer[--i]);
                    }

                    terminal_putchar('.');

                    for (int j = 0; j < 6; j++) {
                        fracPart *= 10;
                        int digit = (int)fracPart;
                        terminal_putchar(digit + '0');
                        fracPart -= digit;
                    }
                    break;
                }
                case '%': {
                    terminal_putchar('%');
                    break;
                }
                default: {
                    terminal_putchar('%');
                    terminal_putchar(*fmt);
                    break;
                }
            }
        } else {
            terminal_putchar(*fmt);
        }
        fmt++;
    }
    
    va_end(args);
}
