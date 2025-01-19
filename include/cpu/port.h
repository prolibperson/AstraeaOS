#ifndef PORT_H
#define PORT_H

#include "global.h"

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

#endif // PORT_H
