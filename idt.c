#include <stdint.h>
#include "console.h"
#include "io.h"
#include "gdt.h"

#define NUM_ENTRIES 256

#define ATTR_PRESENT    1 << 7
#define ATTR_PRIVIL(x)  (x) << 5
#define TYPE_INTERRUPT  0xE
#define TYPE_TRAP       0xF

#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)

#define ICW1_INIT       0x10
#define ICW1_ICW4       0x01
#define ICW4_8086       0x01

#define IRQ_OFFSET      0x20

#define EOI             0x20

// extern void irq0(void);
// extern void irq1(void);
// extern void irq2(void);
// extern void irq3(void);
// extern void irq4(void);
// extern void irq5(void);
// extern void irq6(void);
// extern void irq7(void);
// extern void irq8(void);
// extern void irq9(void);
// extern void irq10(void);
// extern void irq11(void);
// extern void irq12(void);
// extern void irq13(void);
// extern void irq14(void);
// extern void irq15(void);
extern void ignore_interrupt(void);
extern void ignore_exception(void);

// void pic_reset(void) {
//     unsigned char a1, a2;
//     a1 = in8(PIC1_DATA);
//     a2 = in8(PIC2_DATA);

//     out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
//     out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

//     out8(PIC1_DATA, IRQ_OFFSET);
//     out8(PIC2_DATA, IRQ_OFFSET+8);

//     out8(PIC1_DATA, 4);
//     out8(PIC2_DATA, 2);

//     out8(PIC1_DATA, ICW4_8086);
//     out8(PIC2_DATA, ICW4_8086);

//     out8(PIC1_DATA, a1);
//     out8(PIC2_DATA, a2);
// }


// uint64_t idt_entry(uint32_t offset, uint16_t selector) {
//     uint64_t e = 0;
//     uint8_t type_attr = 0x8e;
//     e |= (((uint64_t)offset) & 0xffff) << 48;
//     e |= ((uint64_t)selector) << 32;
//     e |= ((uint64_t)type_attr) << 16;
//     e |= (offset >> 16)& 0xffff;
//     return e;
// }


typedef struct {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
} __attribute__((packed)) idt_entry;

idt_entry idt[256];

void idt_init(void) {
    // pic_reset();

    // idt[0x0]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x1]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x2]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x3]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x4]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x5]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x6]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x7]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x8]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0x9]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0xa]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0xb]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0xc]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0xd]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0xe]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[0xf]           = idt_entry((uint32_t)ignore,code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);

    // idt[IRQ_OFFSET]    = idt_entry((uint32_t)irq0,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+1]  = idt_entry((uint32_t)irq1,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+2]  = idt_entry((uint32_t)irq2,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+3]  = idt_entry((uint32_t)irq3,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+4]  = idt_entry((uint32_t)irq4,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+5]  = idt_entry((uint32_t)irq5,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+6]  = idt_entry((uint32_t)irq6,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+7]  = idt_entry((uint32_t)irq7,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+8]  = idt_entry((uint32_t)irq8,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+9]  = idt_entry((uint32_t)irq9,  code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+10] = idt_entry((uint32_t)irq10, code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+11] = idt_entry((uint32_t)irq11, code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+12] = idt_entry((uint32_t)irq12, code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+13] = idt_entry((uint32_t)irq13, code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+14] = idt_entry((uint32_t)irq14, code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);
    // idt[IRQ_OFFSET+15] = idt_entry((uint32_t)irq15, code_selector, ATTR_PRESENT | ATTR_PRIVIL(0) | TYPE_INTERRUPT);

    uint32_t addr = (uint32_t)ignore_exception;
    idt[0x0D].offset_lowerbits = addr & 0xffff;
    idt[0x0D].selector = 0x10;
    idt[0x0D].zero = 0;
    idt[0x0D].type_attr = 0x8e;
    idt[0x0D].offset_higherbits = (addr & 0xffff0000) >> 16;

    idt[0x0B].offset_lowerbits = addr & 0xffff;
    idt[0x0B].selector = 0x10;
    idt[0x0B].zero = 0;
    idt[0x0B].type_attr = 0x8e;
    idt[0x0B].offset_higherbits = (addr & 0xffff0000) >> 16;

    uint32_t addr2 = (uint32_t)ignore_interrupt;
    idt[0x80].offset_lowerbits = addr2 & 0xffff;
    idt[0x80].selector = 0x10;
    idt[0x80].zero = 0;
    idt[0x80].type_attr = 0x8e;
    idt[0x80].offset_higherbits = (addr2 & 0xffff0000) >> 16;

    // idt[0xD] = idt_entry((uint32_t)ignore_exception, code_selector);
    // idt[0x80] = idt_entry((uint32_t)ignore, code_selector);

    uint16_t descriptor[3];
    descriptor[0] = 256 * 8;
    uint32_t *base = (uint32_t *)&descriptor[1];
    *base = (uint32_t)&idt[0];
    asm volatile("lidt %0" : : "m"(descriptor) : );

    // const int freq = 1000;

    // int counter = 1193182 / freq;
    // out8(0x43, 0x34);
    // out8(0x40,counter & 0xFF);
    // out8(0x40,counter >> 8);
    asm volatile("sti");
}

// void irq0_handler(void) {
//     printk("irq0\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq1_handler(void) {
//     printk("irq1\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq2_handler(void) {
//     printk("irq2\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq3_handler(void) {
//     printk("irq3\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq4_handler(void) {
//     printk("irq4\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq5_handler(void) {
//     printk("irq5\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq6_handler(void) {
//     printk("irq6\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq7_handler(void) {
//     printk("irq7\n");
//     out8(PIC1_COMMAND, EOI);
// }

// void irq8_handler(void) {
//     printk("irq8\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq9_handler(void) {
//     printk("irq9\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq10_handler(void) {
//     printk("irq10\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq11_handler(void) {
//     printk("irq11\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq12_handler(void) {
//     printk("irq12\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq13_handler(void) {
//     printk("irq13\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq14_handler(void) {
//     printk("irq14\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }

// void irq15_handler(void) {
//     printk("irq15\n");
//     out8(PIC1_COMMAND, EOI);
//     out8(PIC2_COMMAND, EOI);
// }