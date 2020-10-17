#include <stdint.h>

void putchar(char c) {
    static uint16_t *buf = (uint16_t *)0xb8000;

    uint8_t color = 0x7; /* light grey foreground, black background */
    *buf = (color << 8) | c;
    buf++;
}

void printk(char *c) {
    while (c) {
        putchar(*c);
        c++;
    }

}

void kmain() {
    printk("hello, world");
    while(1);
}
