global idt_load
extern idt_descriptor

section .text
idt_load:
    cli
    lidt [idt_descriptor]
    sti
    ret
