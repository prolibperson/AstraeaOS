# nuke built-in rules and variables
MAKEFLAGS += -rR
.SUFFIXES:

# toolchain
CC := x86_64-elf-gcc
AS := nasm  # Use nasm for assembly files
LD := x86_64-elf-ld
AR := x86_64-elf-ar

# qemu flags
QEMUFLAGS := -m 2G
IMAGE_NAME := prolibOS

# compiler and linker flags
CFLAGS := -m64 -mcmodel=kernel -ffreestanding -fno-stack-protector -fno-stack-check -nostdlib -O2 -g -Wall -Wextra
LDFLAGS := -T linker.ld -nostdlib -static

# source and asm files
KERNEL_SRC := $(shell find src -type f -name "*.c")
KERNEL_ASM := $(shell find src -type f -name "*.asm")
KERNEL_OBJ := $(patsubst src/%, bin/%, $(KERNEL_SRC:.c=.o) $(KERNEL_ASM:.asm=.o))

# include dirs
INCLUDE_DIRS := $(shell find include -type d)
INCLUDES := $(foreach dir, $(INCLUDE_DIRS), -I$(dir))

# ensure bin and isobuilds directories
$(shell mkdir -p bin isobuilds)

.PHONY: all run-debug run-release debug release clean

# default target
all: release

# compile C files
bin/%.o: src/%.c
	@mkdir -p $(dir $@)  
	$(CC) $(CFLAGS) $(INCLUDES) $(EXTRA_CFLAGS) -c $< -o $@

# compile assembly files with nasm
bin/%.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf64 -o $@ $<

# link kernel
bin/kernel.elf: $(KERNEL_OBJ)
	mkdir -p bin
	$(LD) $(LDFLAGS) -o $@ $^

# build debug iso
isobuilds/prolibOS_debug.iso: EXTRA_CFLAGS := -DDEBUG_BUILD
isobuilds/prolibOS_debug.iso: bin/kernel.elf limine/limine
	rm -rf isodirs
	mkdir -p isodirs/boot/limine
	cp bin/kernel.elf isodirs/boot/
	cp limine.conf isodirs/boot/limine/
	cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isodirs/boot/limine/
	mkdir -p isodirs/EFI/BOOT
	cp limine/BOOTX64.EFI isodirs/EFI/BOOT/
	xorriso -as mkisofs -R -J -b boot/limine/limine-bios-cd.bin -no-emul-boot \
		-boot-load-size 4 -boot-info-table -hfsplus -apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image \
		--protective-msdos-label isodirs -o isobuilds/prolibOS_debug.iso
	./limine/limine bios-install isobuilds/prolibOS_debug.iso
	rm -rf isodirs

# build release iso
isobuilds/prolibOS.iso: bin/kernel.elf limine/limine
	rm -rf isodirs
	mkdir -p isodirs/boot/limine
	cp bin/kernel.elf isodirs/boot/
	cp limine.conf isodirs/boot/limine/
	cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isodirs/boot/limine/
	mkdir -p isodirs/EFI/BOOT
	cp limine/BOOTX64.EFI isodirs/EFI/BOOT/
	xorriso -as mkisofs -R -J -b boot/limine/limine-bios-cd.bin -no-emul-boot \
		-boot-load-size 4 -boot-info-table -hfsplus -apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image \
		--protective-msdos-label isodirs -o isobuilds/prolibOS.iso
	./limine/limine bios-install isobuilds/prolibOS.iso
	rm -rf isodirs

# build limine
limine/limine:
	rm -rf limine
	git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1
	make -C limine

# cleanup
clean:
	rm -rf bin $(KERNEL_OBJ) isobuilds/prolibOS_debug.iso isobuilds/prolibOS.iso limine isodirs

clean_no_iso:
	rm -rf bin $(KERNEL_OBJ) limine isodirs

# separate build targets
debug: isobuilds/prolibOS_debug.iso

release: isobuilds/prolibOS.iso

# run in qemu
run-debug: clean_no_iso debug
	qemu-system-x86_64 -M q35 -cdrom isobuilds/prolibOS_debug.iso $(QEMUFLAGS)

run-release: clean_no_iso release
	qemu-system-x86_64 -M q35 -cdrom isobuilds/prolibOS.iso $(QEMUFLAGS)

qemu-debug:
	qemu-system-x86_64 -M q35 -cdrom isobuilds/prolibOS_debug.iso $(QEMUFLAGS)

qemu-release:
	qemu-system-x86_64 -M q35 -cdrom isobuilds/prolibOS.iso $(QEMUFLAGS)