#include <stdint.h>

#define NUM_ENTRIES 3
#define MAX_LIMIT 0xfffff

#define FLAG_GRANULARITY    1 << 3
#define FLAG_SIZE           1 << 2
// #define FLAG_LONG_MODE      1 << 1

#define ACCESS_PRESENT      1 << 7
#define ACCESS_PRIV_KERNEL  0
// #define ACCESS_PRIV_USER    3 << 5
#define ACCESS_SEGMENT      1 << 4
#define ACCESS_EXEC         1 << 3
// #define ACCESS_CONFORMING   1 << 2
#define ACCESS_RW           1 << 1



typedef struct gdt_entry {
    uint16_t limit_lowerbits;
    uint16_t base_lowerbits;
    uint8_t base_middlebits;
    uint8_t access;
    uint8_t limit_higherbits : 4;
    uint8_t flags : 4;
    uint8_t base_higherbits;
} __attribute__((packed)) gdt_entry;

static gdt_entry gdt[NUM_ENTRIES];

static gdt_entry entry(uint32_t base, uint32_t limit, uint32_t flags, uint32_t access) {
    gdt_entry ent;
    ent.limit_lowerbits = limit & 0xffff;
    ent.base_lowerbits = base & 0xffff;
    ent.base_middlebits = (base >> 16) & 0xff;
    ent.access = access;
    ent.limit_higherbits = (limit >> 16) & 0xf;
    ent.flags = flags;
    ent.base_higherbits = (base >> 24) & 0xff;
    return ent;
}

void gdt_init(void) {
    /* null descriptor */
    gdt[0] = entry(0, 0, 0, 0);
    /* data segment */
    gdt[1] = entry(0, MAX_LIMIT, FLAG_GRANULARITY | FLAG_SIZE, ACCESS_PRESENT | ACCESS_PRIV_KERNEL | ACCESS_SEGMENT | ACCESS_RW);
    /* code segment */
    gdt[2] = entry(0, MAX_LIMIT, FLAG_GRANULARITY | FLAG_SIZE, ACCESS_PRESENT | ACCESS_PRIV_KERNEL | ACCESS_SEGMENT | ACCESS_EXEC | ACCESS_RW);

    uint16_t descriptor[3];
    descriptor[0] = NUM_ENTRIES * 8;
    uint32_t *base = (uint32_t *)&descriptor[1];
    *base = (uint32_t)gdt;
    asm volatile("lgdt %0" : : "m"(descriptor) : );
    // TODO: set cs after gdt load

}