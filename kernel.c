#include "multiboot.h"
#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "mem.h"

void kmain(multiboot_info_t *mbd, uint32_t magic) {
    console_init();
    gdt_init();
    idt_init();
    mem_init(mbd, magic);
    
    printk("everything initialized\n");
    asm volatile("int $0x80");
    printk("returned from interrupt\n");
    uint32_t *a = kmalloc(sizeof(uint32_t));
    uint32_t *b = kmalloc(sizeof(uint32_t));
    printkl((uint32_t)a);
    putchar('\n');
    printkl((uint32_t)b);
    putchar('\n');
    while(1);
}
