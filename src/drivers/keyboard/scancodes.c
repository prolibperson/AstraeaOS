#include "scancodes.h"

const char scancode_to_char[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', // Backspace
    '\t', // Tab
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // Enter
    0,  // Left Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,  // Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // Right Shift
    '*', 0, // Alt
    ' ', // Space
    0,  // Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
    0,  // Num Lock
    0,  // Scroll Lock
    0,  // Home key
    0x48,  // Up Arrow
    0,  // Page Up
    '-',
    0x4B,  // Left Arrow
    0,  // Center
    0x4D,  // Right Arrow
    '+',
    0,  // End key
    0x50,  // Down Arrow
    0,  // Page Down
    0,  // Insert key
    0,  // Delete key
    0, 0, 0, 0, 0, 0, 0, // F11-F12
    0  // Reserved keys
};

const char scancode_to_char_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', // Backspace
    '\t', // Tab
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', // Enter
    0,  // Left Control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,  // Left Shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, // Right Shift
    '*', 0, // Alt
    ' ', // Space
    0,  // Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
    0,  // Num Lock
    0,  // Scroll Lock
    0,  // Home key
    0x48,  // Up Arrow
    0,  // Page Up
    '-',
    0x4B,  // Left Arrow
    0,  // Center
    0x4D,  // Right Arrow
    '+',
    0,  // End key
    0x50,  // Down Arrow
    0,  // Page Down
    0,  // Insert key
    0,  // Delete key
    0, 0, 0, 0, 0, 0, 0, // F11-F12
    0  // Reserved keys
};