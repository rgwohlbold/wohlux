#include "console.h"
#include "gdt.h"
#include "idt.h"

void kmain() {
    console_init();
    gdt_init();
    idt_init();
    printk("everything initialized\n");
    asm volatile("int $0x80");
    printk("returned from interrupt\n");
    while(1);
}
