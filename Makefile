# HELIOS 1.0 Makefile
# Build system for complete operating system

CC = x86_64-elf-gcc
CXX = x86_64-elf-g++
AS = nasm
LD = x86_64-elf-ld

# Compiler flags
CFLAGS = -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -Werror -O0 -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++14
ASFLAGS_BIN = -f bin        # For 16-bit bootloader (raw binary)
ASFLAGS_ELF = -f elf64      # For 64-bit code (ELF format)

# Linker flags
LDFLAGS = -T linker.ld -nostdlib

# Object files
KERNEL_OBJS = \
	build/kernel/kernel.o \
	build/kernel/kernel_asm.o \
	build/fs/filesystem.o \
	build/shell/shell.o \
	build/gui/gui.o \
	build/browser/browser.o

BOOTLOADER = build/bootloader/boot.bin
TARGET = build/kernel.elf
ISO_OUTPUT = build/helios.iso

.PHONY: all build iso run-qemu clean help

all: build iso

# Build kernel and bootloader
build: $(BOOTLOADER) $(TARGET)
	@echo "[OK] Build complete!"

# Bootloader (16-bit binary - must use -f bin, not -f elf64)
$(BOOTLOADER): bootloader/boot.asm
	@mkdir -p build/bootloader
	$(AS) $(ASFLAGS_BIN) -o $@ $<
	@echo "[OK] Bootloader compiled: $@"

# Kernel C++
build/kernel/kernel.o: kernel/kernel.cpp
	@mkdir -p build/kernel
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "[OK] Kernel C++ compiled"

# Kernel Assembly
build/kernel/kernel_asm.o: kernel/kernel.asm
	@mkdir -p build/kernel
	$(AS) $(ASFLAGS_ELF) -o $@ $<
	@echo "[OK] Kernel assembly compiled"

# File System
build/fs/filesystem.o: fs/filesystem.cpp
	@mkdir -p build/fs
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "[OK] File system compiled"

# Shell
build/shell/shell.o: shell/shell.cpp
	@mkdir -p build/shell
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "[OK] Shell compiled"

# GUI
build/gui/gui.o: gui/gui.cpp
	@mkdir -p build/gui
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "[OK] GUI compiled"

# Browser
build/browser/browser.o: browser/browser.cpp
	@mkdir -p build/browser
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "[OK] Browser compiled"

# Link kernel
$(TARGET): $(KERNEL_OBJS)
	@mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "[OK] Kernel linked: $@"

# Create ISO
iso: build
	@mkdir -p build/iso/boot/grub
	@cp $(BOOTLOADER) build/iso/boot/
	@cp $(TARGET) build/iso/boot/kernel.elf
	@cp grub.cfg build/iso/boot/grub/
	@echo "[*] Creating ISO image..."
	@grub-mkrescue -o $(ISO_OUTPUT) build/iso/ 2>/dev/null || \
		mkisofs -o $(ISO_OUTPUT) -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table build/iso/ 2>/dev/null || \
		xorriso -as mkisofs -o $(ISO_OUTPUT) -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table build/iso/ 2>/dev/null
	@if [ -f $(ISO_OUTPUT) ]; then echo "[OK] ISO created: $(ISO_OUTPUT)"; else echo "[ERROR] ISO creation failed"; exit 1; fi

# Run in QEMU
run-qemu: iso
	@echo "[*] Booting HELIOS in QEMU..."
	@qemu-system-x86_64 -cdrom $(ISO_OUTPUT) -m 512 -display gtk 2>/dev/null || \
		qemu-system-x86_64 -cdrom $(ISO_OUTPUT) -m 512 -display sdl 2>/dev/null || \
		qemu-system-x86_64 -cdrom $(ISO_OUTPUT) -m 512

# Clean build files
clean:
	@rm -rf build/
	@echo "[OK] Build cleaned"

# Help
help:
	@echo "HELIOS 1.0 Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make build      - Build kernel and bootloader"
	@echo "  make iso        - Create bootable ISO"
	@echo "  make run-qemu   - Run in QEMU emulator"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make help       - Show this help"
	@echo ""
