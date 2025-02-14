#include "cpu_manager.h"

/* halt */
void halt(void) {
    for (;;) {
        asm volatile("hlt");
    }
}

/* rng */
static unsigned int seed = 123456789; /* seed */
unsigned int rand() {
    seed = (1103515245 * seed + 12345) & 0x7fffffff;
    return seed;
}