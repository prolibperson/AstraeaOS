global idt_load
extern idt_descriptor

section .text
bits 64
idt_load:
    cli
    lidt [idt_descriptor]
    ret