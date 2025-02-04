section .multiboot_header
header_start:
    ; Multiboot2 header
    dd 0xE85250D6                ; Magic number (Multiboot2)
    dd 0                         ; Architecture (0 for x86)
    dd header_end - header_start ; Header length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start)) ; Checksum

    ; End tag (required)
    dw 0                         ; Type: End tag
    dw 0                         ; Flags
    dd 8                         ; Size: 8 bytes

header_end: