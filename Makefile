AS = x86_64-elf-as
CC = x86_64-elf-gcc
GRUB_MKRESCUE = grub-mkrescue
QEMU = qemu-system-x86_64
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
ISO_FILE = isobuilds/prolibos_beta.iso
ISO_FILE_RELEASE = isobuilds/prolibos.iso

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(addprefix -I, $(shell find $(INCLUDE_DIR) -type d))
LDFLAGS = -ffreestanding -O2 -nostdlib

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean run release run-release clean-no-iso

all: clean-no-iso $(ISO_FILE)

$(ISO_FILE): CFLAGS += -DDEBUG_BUILD

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OUTPUT_BIN): $(OBJECTS) $(ASM_OBJECTS) $(LINKER_SCRIPT)
	$(CC) -T $(LINKER_SCRIPT) -o $(OUTPUT_BIN) $(LDFLAGS) $(OBJECTS) $(ASM_OBJECTS) -lgcc

$(ISO_FILE): $(OUTPUT_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OUTPUT_BIN) $(ISO_DIR)/boot/prolibos.bin
	echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "prolibOS" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    multiboot2 /boot/prolibos.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

$(ISO_FILE_RELEASE): $(OUTPUT_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OUTPUT_BIN) $(ISO_DIR)/boot/prolibos.bin
	echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "prolibOS" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    multiboot2 /boot/prolibos.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

release: $(ISO_FILE_RELEASE)
	@echo "Release ISO created: $(ISO_FILE_RELEASE)"

run: all
	$(QEMU) -cdrom $(ISO_FILE)

run-release: release
	$(QEMU) -cdrom $(ISO_FILE_RELEASE)

clean-no-iso:
	rm -rf $(BUILD_DIR)/* $(ISO_DIR) prolibos.bin

clean:
	rm -rf $(BUILD_DIR)/* $(ISO_FILE) $(ISO_FILE_RELEASE) $(ISO_DIR) prolibos.bin beta/*
