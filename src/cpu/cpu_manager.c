#include "cpu_manager.h"

/* halt */
void halt(void) {
    for (;;) {
        asm volatile("hlt");
    }
}