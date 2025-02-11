global gdt_load
extern gdt_descriptor

section .text
bits 64
gdt_load:
    lgdt [rel gdt_descriptor]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push 0x08
    lea rax, [rel .flush]
    push rax
    retfq

.flush:
    ret
