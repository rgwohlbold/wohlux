#ifndef CONSOLE_H
#define CONSOLE_H
#include <stdint.h>

void putchar(char c);
void printk(char *c);
void printkbin(void *v, uint32_t size);
void printkb(uint8_t b);
void printkw(uint16_t w);
void printkl(uint32_t l);
void printkq(uint64_t q);

void console_init(void);

#endif