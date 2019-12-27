#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

//Output byte into specified port.
extern void outb(uint16_t port, uint8_t byte);

//Recieve byte from specified port.
extern uint8_t inb(uint16_t port);

//Output double-byte into specified port.
extern void outw(uint16_t port, uint16_t word);

//Recieve double-byte from specified port.
extern uint16_t inw(uint16_t port);

//Output long into specified port.
extern void outl(uint16_t port, uint32_t dualword);

//Recieve long from specified port.
extern uint32_t inl(uint16_t port);

#endif