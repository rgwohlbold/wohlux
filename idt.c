#include <stdint.h>
#include "console.h"
#include "io.h"
#include "gdt.h"

#define NUM_ENTRIES     256

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

#define PIT_FREQ        0

extern void irq0x00(void);
extern void irq0x01(void);
extern void isr0x0B(void);
extern void isr0x0D(void);
extern void isr0x80(void);

void pic_reset(void) {
    unsigned char a1, a2;
    a1 = in8(PIC1_DATA);
    a2 = in8(PIC2_DATA);

    out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    out8(PIC1_DATA, IRQ_OFFSET);
    out8(PIC2_DATA, IRQ_OFFSET+8);

    out8(PIC1_DATA, 4);
    out8(PIC2_DATA, 2);

    out8(PIC1_DATA, ICW4_8086);
    out8(PIC2_DATA, ICW4_8086);

    out8(PIC1_DATA, a1);
    out8(PIC2_DATA, a2);
}

void pit_init(uint32_t freq) {
    uint32_t counter;
    if (freq != 0)
        counter = 1193182 / freq;
    else
        counter = 0;
    
    out8(0x43, 0x34);
    out8(0x40,counter & 0xFF);
    out8(0x40,counter >> 8);
}


typedef struct {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
} __attribute__((packed)) idt_entry;

idt_entry idt[256];

void set_interrupt_gate(uint8_t number, void (*handler)()) {
    uint32_t addr = (uint32_t)handler;
    idt[number].offset_lowerbits = addr & 0xffff;
    idt[number].selector = 0x10;
    idt[number].zero = 0;
    idt[number].type_attr = 0x8e;
    idt[number].offset_higherbits = (addr & 0xffff0000) >> 16;
}

void load_idt() {
    uint16_t descriptor[3];
    descriptor[0] = 256 * 8;
    uint32_t *base = (uint32_t *)&descriptor[1];
    *base = (uint32_t)&idt[0];
    asm volatile("lidt %0" : : "m"(descriptor) : );
}

void idt_init(void) {
    set_interrupt_gate(0x0d, isr0x0D);
    set_interrupt_gate(0x0b, isr0x0B);
    set_interrupt_gate(0x80, isr0x80);
    set_interrupt_gate(0x20, irq0x00);
    set_interrupt_gate(0x21, irq0x01);
    load_idt();

    pic_reset();
    pit_init(PIT_FREQ);

    asm volatile("sti");
}


typedef struct interrupt_data {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t number;
    uint32_t error;
} __attribute__((packed)) interupt_data;

void handle_interrupt(struct interrupt_data data) {

    switch (data.number) {
        case IRQ_OFFSET + 0x00: /* pit */
            break;
        case IRQ_OFFSET + 0x01: ; 
            uint8_t key = in8(0x60);
            printk("keyboard: 0x");
            printkb(key);
            putchar('\n');
            break;
        default:
            printk("received interrupt 0x");
            printkb((uint8_t)data.number);
            printk(" with error code 0x");
            printkl(data.error);
            putchar('\n');
    }

    if (data.number >= 0x20 && data.number < 0x30) {
        out8(PIC1_COMMAND, EOI);
        if (data.number >= 0x28)
            out8(PIC2_COMMAND, EOI);
    }
}