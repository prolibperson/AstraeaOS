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
    # Load the stack pointer
    mov $stack_top, %esp

    # Prepare for long mode
    cli                         # Disable interrupts
    mov %cr0, %eax              # Load CR0
    or $0x1, %eax               # Set the protected mode bit
    mov %eax, %cr0              # Write back to CR0
    jmp $8, $.protected_mode    # Far jump to set CS and enter protected mode

.protected_mode:
    mov $0x10, %ax              # Load data segment selector (GDT setup needed)
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Enable long mode
    mov $0xC0000080, %ecx       # Model Specific Register (MSR) for long mode
    rdmsr                       # Read MSR
    or $(1 << 8), %eax          # Set the long mode enable bit
    wrmsr                       # Write back to MSR

    mov %cr4, %eax              # Load CR4
    or $(1 << 5), %eax          # Enable Physical Address Extensions (PAE)
    mov %eax, %cr4              # Write back to CR4

    mov %cr3, %eax              # Load CR3 (needs paging setup)
    mov %eax, %cr3              # Activate paging tables

    mov %cr0, %eax              # Reload CR0
    or $(1 << 31), %eax         # Enable paging
    mov %eax, %cr0              # Write back to CR0

    # Enter long mode
    jmp $0x28, $.long_mode      # Far jump to set CS and enter long mode

.long_mode:
    mov $0x30, %ax              # Data segment selector for 64-bit mode
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Call the kernel entry point
    call kernel_main

    # Halt the CPU
1:  cli
    hlt
    jmp 1b

.size _start, . - _start
