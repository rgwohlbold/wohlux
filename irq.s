.set IRQ_OFFSET, 0x20
.section .text
.extern handle_interrupt

.macro irq n
.global irq\n
irq\n:
    pushl $0
    pushl $IRQ_OFFSET + \n
    jmp int_common
.endm

.macro isr_error n
.global isr\n
isr\n:
    pushl $\n
    jmp int_common
.endm

.macro isr n
.global isr\n
isr\n:
    pushl $0
    pushl $\n
    jmp int_common
.endm

int_common:
    pusha
    call handle_interrupt
    popa
    add $8, %esp
    iret

irq 0x00
irq 0x01
isr_error 0x0B
isr_error 0x0D
isr 0x80

