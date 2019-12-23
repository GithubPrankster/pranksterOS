#include "kernel.h"

void outb(unsigned short port, unsigned char byte){
    asm volatile ( "outb %0, %1" : : "a"(byte), "Nd"(port) );
}

uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}