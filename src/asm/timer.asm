section .text
global timer_handler_stub

extern timer_handler
extern terminal_toggle_cursor

timer_handler_stub:
    ; Save registers
    push rax
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Call the C handler
    mov rdi, 0        ; IRQ0 (timer) does not have an error code
    call timer_handler

    ; Toggle the cursor visibility every interrupt (0.5s or 1s based on timer frequency)
    call terminal_toggle_cursor

    ; Restore registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    ; Return from interrupt
    iretq
