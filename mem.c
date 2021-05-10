#include <stddef.h>
#include "mem.h"
#include "console.h"

typedef struct mmap_entry {
    uint32_t size;
    // uint32_t base_addr_low;
    // uint32_t base_addr_high;
    uint64_t base_addr;
    // uint32_t length_low;
    // uint32_t length_high;
    uint64_t length;
    uint32_t type;
} __attribute__((packed)) mmap_entry;

typedef struct mhdr {
    struct mhdr *prev;
    struct mhdr *next;
    uint32_t size;
    uint8_t used;
    uint8_t *data;
} mhdr;

mhdr *first;
mhdr *last;

void mem_init(multiboot_info_t *mbd, uint32_t magic) {
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
            int32_t size = mbd->mmap_length;
            mmap_entry *ent = (mmap_entry *)mbd->mmap_addr;
            while (size > 0) {
                /* if available and large enough, give to memory manager */
                if (ent->type == MULTIBOOT_MEMORY_AVAILABLE && ent->length > sizeof(mhdr) && ent->base_addr < UINT32_MAX) {

                    // TODO check that all memory is addressable
                    mhdr *hdr;
                    if (ent->base_addr) {
                        hdr = (mhdr *)((uint32_t)(ent->base_addr));
                        hdr->size = ent->size - sizeof(mhdr);
                    } else {
                        /* if base_addr is NULL, allocate at 0x1 */
                        hdr = (mhdr *)((uint32_t)(ent->base_addr+1));
                        hdr->size = ent->size - sizeof(mhdr) - 1;
                    }
                    hdr->prev = last;
                    hdr->next = NULL;
                    hdr->data = (uint8_t *)(hdr+1);
                    hdr->used = 0;
                    last->next = hdr;
                    last = hdr;
                    if (!first) {
                        first = hdr;
                    }

                    printk("region at 0x");
                    printkq(ent->base_addr);
                    printk(" of length 0x");
                    printkq(ent->length);
                    printk(" and type ");
                    printkb(ent->type);
                    putchar('\n');
                }
                // size does not count to size
                size -= ent->size + 4;
                ent = (mmap_entry *) ((uint32_t)ent + ent->size + 4); 
            }
        }
        else {
            printk("complete memory map not present\n");
            if (mbd->flags & MULTIBOOT_INFO_MEMORY)
                printk("conventional memory map present\n");
            else 
                printk("conventional memory map not present\n");
        
        }
    } else {
        printk("magic number not found");
    }
}

void *kmalloc(uint32_t size) {
    for (mhdr *hdr = first; hdr != NULL; hdr = hdr->next) {
        if (hdr->size >= size && !hdr->used) {
            hdr->used = 1; 
            if (hdr->size > size + sizeof(mhdr)) {
                mhdr *new = (mhdr *)(hdr->data + size);
                new->size = hdr->size - size - sizeof(mhdr);
                new->data = (uint8_t *) (new+1);
                new->used = 0;
                new->prev = hdr;
                new->next = hdr->next;
                if (hdr == last)
                    last = new;

                hdr->size = size;
                hdr->next = new;
            }
            return (void *)hdr->data;
        }
    }
    return NULL;
}
