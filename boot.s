.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .text
.extern kmain
.global _start
.type _start, @function
_start:
    mov $stack, %esp
    push %eax
    push %ebx
    call kmain

    cli
1:
    hlt
    jmp 1b

.section .bss
.align 16
.skip 2 * 1024 * 1024
stack:
