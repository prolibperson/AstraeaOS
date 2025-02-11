global exception_wrappers
extern exception_handler

section .text
bits 64
exception_wrappers:
    %assign i 0
    %rep 32
        push rdi
        mov rdi, i
        call exception_handler
        pop rdi
        iretq
        %assign i i + 1
    %endrep
