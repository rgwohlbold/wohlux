#include <stdint.h>

#define NUM_ENTRIES 3
#define MAX_LIMIT 0xfffff

#define FLAG_GRANULARITY    1 << 3
#define FLAG_SIZE           1 << 2
// #define FLAG_LONG_MODE      1 << 1

#define ACCESS_PRESENT      1 << 7
#define ACCESS_PRIV_KERNEL  0
#define ACCESS_PRIV_USER    3 << 5
#define ACCESS_SEGMENT      1 << 4
#define ACCESS_EXEC         1 << 3
#define ACCESS_CONFORMING   1 << 2
#define ACCESS_RW           1 << 1


static uint64_t gdt[NUM_ENTRIES];

static uint64_t gdt_entry(uint32_t base, uint32_t limit, uint32_t flags, uint32_t access) {
    uint64_t e = 0;
    e |= limit & 0xffffLL;
    e |= (base & 0xffffffLL) << 16;
    e |= (uint64_t)access << 40;
    e |= ((limit >> 16) & 0xfLL) << 48;
    e |= (flags & 0xfLL) << 52;
    e |= ((base >> 24) & 0xffLL) << 56;
    return e;
}

void gdt_init(void) {
    /* null descriptor */
    gdt[0] = 0;
    /* code segment */
    gdt[1] = gdt_entry(0, MAX_LIMIT, FLAG_GRANULARITY | FLAG_SIZE, ACCESS_PRESENT | ACCESS_PRIV_KERNEL | ACCESS_SEGMENT | ACCESS_EXEC | ACCESS_RW);
    /* data segment */
    gdt[2] = gdt_entry(0, MAX_LIMIT, FLAG_GRANULARITY | FLAG_SIZE, ACCESS_PRESENT | ACCESS_PRIV_KERNEL | ACCESS_SEGMENT | ACCESS_RW);

    uint16_t descriptor[3];
    descriptor[0] = NUM_ENTRIES * 8;
    uint32_t *base = (uint32_t *)&descriptor[1];
    *base = (uint32_t)&gdt;
    asm volatile("lgdt (%0)" : : "r"(&descriptor) : );

}