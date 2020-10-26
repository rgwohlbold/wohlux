#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include "multiboot.h"

void mem_init(multiboot_info_t *mbd, uint32_t magic);
void *kmalloc(uint32_t size);

#endif