section .multiboot_header
header_start:
    dd 0xE85250D6                ; magic number (multiboot2)
    dd 0                         ; architecture (0 for x86)
    dd header_end - header_start ; header length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start)) ; checksum

    ; end tag
    dw 0
    dw 0
    dd 8

header_end: