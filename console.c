#include <stdint.h>
#include "console.h"

#define VGA(color, c) ((color) << 8) | (c)

static uint16_t *buf = (uint16_t *)0xb8000;
static uint8_t color = 0x7; /* light grey foreground, black background */

void putchar(char c) {
    static uint8_t x = 0;
    static uint8_t y = 0;
    if (c != '\n')
        buf[80*y+x] = VGA(color, c);
    
    if (x == 79 || c == '\n') {
        x = 0;
        if (y == 24) {
            for (int i = 0; i < 25; i++) {
                for (int j = 0; j < 80; j++) {
                    buf[80*i+j] = buf[80*(i+1)+j];
                }
            }
        } else {
            y++;
        }
    } else {
        x++;
    }

}

void printk(char *c) {
    while (*c) {
        putchar(*c);
        c++;
    }

}

void console_init(void) {
    for (int i = 0; i < 25*80; i++) {
        buf[i] = VGA(color, ' ');
    }
}