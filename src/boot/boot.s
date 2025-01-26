.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align   4
.long    MAGIC
.long    FLAGS
.long    CHECKSUM

.section .bss
.align   16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp

    cli
    mov %cr0, %eax
    or $0x1, %eax
    mov %eax, %cr0
    jmp $8, $.protected_mode

.protected_mode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    mov $0xC0000080, %ecx
    rdmsr
    or $(1 << 8), %eax
    wrmsr

    mov %cr4, %eax
    or $(1 << 5), %eax
    mov %eax, %cr4

    mov %cr3, %eax
    mov %eax, %cr3

    mov %cr0, %eax
    or $(1 << 31), %eax
    mov %eax, %cr0

    jmp $0x28, $.long_mode

.long_mode:
    mov $0x30, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    call kernel_main

1:  cli
    hlt
    jmp 1b

.size _start, . - _start
