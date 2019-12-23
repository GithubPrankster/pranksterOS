#include "kernel.h"

void outb(unsigned char byte, unsigned short port){
    asm volatile ( "outb %0, %1" : : "a"(byte), "Nd"(port) );
}