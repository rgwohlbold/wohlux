#include "console.h"

void kmain() {
    console_init();
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10; j++) {
            putchar('0' + j);
            putchar('\n');
        }
    }
    while(1);
}
