#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

extern void outb(unsigned short port, unsigned char byte);
extern uint8_t inb(uint16_t port);

#endif