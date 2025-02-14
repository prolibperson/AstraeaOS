#include "limine_manager.h"
#include "cpu_manager.h"
#include "limine.h"
//#include "serial.h"  // Assuming you have a serial port driver for debugging

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

/* framebuffer request */
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
};

/* init limine */
void limine_init(void) {
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
       // serial_printf("Limine base revision not supported\n");
        halt();
    }

    if (framebuffer_request.response == NULL) {
        //serial_printf("Framebuffer request response is NULL\n");
        halt();
    }

    if (framebuffer_request.response->framebuffer_count < 1) {
       // serial_printf("No framebuffers available\n");
        halt();
    }

   // serial_printf("Limine initialized successfully\n");
}

/* fetch framebuffer */
struct limine_framebuffer *get_framebuffer(void) {
    return framebuffer_request.response->framebuffers[0];
}