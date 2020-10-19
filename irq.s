.section .text
.global ignore_interrupt
.global ignore_exception

ignore_exception:
    add $4, %esp
ignore_interrupt:
    iret

