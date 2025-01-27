global exception_wrappers
extern exception_handler

section .text
exception_wrappers:
    %assign i 0
    %rep 32
        push i
        jmp exception_handler
        %assign i i + 1
    %endrep