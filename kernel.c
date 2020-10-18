#include "console.h"
#include "gdt.h"

void kmain() {
    console_init();
    gdt_init();
    printk("gdt initialized");
    while(1);
}
