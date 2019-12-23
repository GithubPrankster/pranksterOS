#include "kernel.h"

void outb(unsigned short port, unsigned char byte){
    asm volatile ( "outb %0, %1" : : "a"(byte), "Nd"(port) );
}