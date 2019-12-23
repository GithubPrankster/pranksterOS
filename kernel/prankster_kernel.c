#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "plibc/string.h"
#include "plibc/kernel.h"

#if defined(__linux__)
#error "You may not know what you're doing."
#endif

#if !defined(__i386__)
#error "You need the cross-compiler for this."
#endif

typedef enum {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_LIGHT_BROWN = 14,
    VGA_WHITE = 15
}vga_col;

static inline uint8_t vga_entry_col(vga_col foreground, vga_col background){
    return foreground | background << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t col){
    return (uint16_t)uc | (uint16_t)col << 8;
}

static const size_t CONSOLE_WIDTH = 80, CONSOLE_HEIGHT = 25;

size_t term_row, term_column;
uint8_t term_col;
uint16_t* term_buffer;

void term_init(void){
    term_row = 0;
    term_column = 0;
    term_col = vga_entry_col(VGA_LIGHT_GREY, VGA_BLACK);
    term_buffer = (uint16_t*)0xB8000;
    for(size_t y = 0; y < CONSOLE_HEIGHT; y++){
        for(size_t x = 0; x < CONSOLE_WIDTH; x++){
            const size_t index = y * CONSOLE_WIDTH + x;
            term_buffer[index] = vga_entry(' ', term_col);
        }
    }
}

void term_setcol(uint8_t col){
    term_col = col;
}

void term_update_cursor(unsigned int x, unsigned int y){
    uint16_t pos = y * CONSOLE_WIDTH + x;
    
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void term_putentryat(char c, uint8_t col, size_t x, size_t y){
    term_update_cursor(x, y);
    const size_t index = y * CONSOLE_WIDTH + x;
    term_buffer[index] = vga_entry(c, col);
}

int term_putchar(char c){
    if(c == '\n'){
        term_column = 0;
        if(++term_row == CONSOLE_HEIGHT)
            term_row = 0;
        return 0;
    }

    term_putentryat(c, term_col, term_column, term_row);
    if(++term_column == CONSOLE_WIDTH){
        term_column = 0;
        if(++term_row == CONSOLE_HEIGHT)
            term_row = 0;
    }
    return 0;
}

void term_write(const char* data, size_t size){
    for(size_t i = 0; i < size; i++)
        term_putchar(data[i]);
}

void term_writestring(const char* data){
    term_write(data, strlen(data));
}

void term_splashscreen(void){
    term_setcol(vga_entry_col(VGA_WHITE, VGA_DARK_GREY));
    term_writestring("****              ****\n");
    term_writestring("**            * *   **\n");
    term_writestring("**           * * *  **\n");
    term_writestring("**            * *   **\n");
    term_writestring("**                  **\n");
    term_writestring("**     *            **\n");
    term_writestring("**    ***           **\n");
    term_writestring("**  *******         **\n");
    term_writestring("**    ***           **\n");
    term_writestring("**     *            **\n");
    term_writestring("****              ****\n\n");
    term_setcol(vga_entry_col(VGA_LIGHT_GREY, VGA_BLACK));
    term_writestring("PranksterOS Ver. dev.0.3\nCreated by Uneven Prankster.\nHow much wood could a woodchuck chuck,\n");
    term_writestring("If a woodchuck could chuck wood?\n");
}

void kernel_main(void){
    term_init();
    term_splashscreen();
}