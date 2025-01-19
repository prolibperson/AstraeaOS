global keyboard_handler_stub
extern keyboard_handler_c

section .text
keyboard_handler_stub:
    pusha
    call keyboard_handler_c
    popa
    iret