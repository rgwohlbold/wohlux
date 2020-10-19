#ifndef GDH_H
#define GDH_H

#include <stdint.h>

void gdt_init(void);
uint16_t gdt_selector(uint16_t number);

#endif