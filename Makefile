AS = i686-elf-as
CC = i686-elf-gcc
GRUB_MKRESCUE = grub-mkrescue
QEMU = qemu-system-i386
NASM = nasm

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
ISO_DIR = isodir
BOOT_FILE = $(BUILD_DIR)/boot.o
ASM_FILES = $(shell find $(SRC_DIR) -name '*.asm')
ASM_OBJECTS = $(ASM_FILES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)
LINKER_SCRIPT = $(SRC_DIR)/boot/linker.ld
OUTPUT_BIN = $(BUILD_DIR)/prolibos.bin
ISO_FILE = prolibosBETA.iso
ISO_FILE_RELEASE = release/prolibos.iso
GRUB_CFG = $(SRC_DIR)/boot/grub.cfg

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(addprefix -I, $(shell find $(INCLUDE_DIR) -type d))
LDFLAGS = -ffreestanding -O2 -nostdlib

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean run release run-release

all: clean $(ISO_FILE)

$(ISO_FILE): CFLAGS += -DDEBUG_BUILD

$(BOOT_FILE): $(SRC_DIR)/boot/boot.s
	$(AS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OUTPUT_BIN): $(BOOT_FILE) $(OBJECTS) $(ASM_OBJECTS) $(LINKER_SCRIPT)
	$(CC) -T $(LINKER_SCRIPT) -o $@ $(LDFLAGS) $(BOOT_FILE) $(OBJECTS) $(ASM_OBJECTS) -lgcc

$(ISO_FILE): $(OUTPUT_BIN) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OUTPUT_BIN) $(ISO_DIR)/boot/prolibos.bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

$(ISO_FILE_RELEASE): clean $(OUTPUT_BIN) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OUTPUT_BIN) $(ISO_DIR)/boot/prolibos.bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

release: $(ISO_FILE_RELEASE)
	@echo "Release ISO created: $(ISO_FILE_RELEASE)"

run: all
	$(QEMU) -cdrom $(ISO_FILE)

run-release: release
	$(QEMU) -cdrom $(ISO_FILE_RELEASE)

clean:
	rm -rf $(BUILD_DIR)/* $(ISO_FILE) $(ISO_FILE_RELEASE) $(ISO_DIR) prolibos.bin
