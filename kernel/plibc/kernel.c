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

void outw(uint16_t port, uint16_t word){
    asm volatile ( "outw %0, %1" : : "a"(word), "Nd"(port) );
}

uint16_t inw(uint16_t port){
    uint16_t ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}


void outl(uint16_t port, uint32_t dualword){
    asm volatile ( "outl %0, %1" : : "a"(dualword), "Nd"(port) );
}

uint32_t inl(uint16_t port){
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}