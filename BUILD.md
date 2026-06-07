# HELIOS 1.0 - Build Instructions

## Prerequisites

Install the required tools:

### On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential nasm grub-pc-bin xorriso qemu-system-x86 git
```

### On Fedora/RHEL:
```bash
sudo dnf install gcc gcc-c++ nasm grub2-tools xorriso qemu-system-x86 git
```

### On macOS:
```bash
brew install nasm grub xorriso qemu git
```

## Cross-Compiler Setup

If you don't have a cross-compiler installed:

### Option 1: Install Pre-built Cross-Compiler

**Ubuntu/Debian:**
```bash
sudo apt-get install gcc-x86-64-linux-gnu g++-x86-64-linux-gnu
```

### Option 2: Build From Source

```bash
# Set installation prefix
export PREFIX="/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

# Download sources
wget https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.xz
wget https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.xz

# Extract
tar xf binutils-2.37.tar.xz
tar xf gcc-11.2.0.tar.xz

# Build binutils
cd binutils-2.37
./configure --target=$TARGET --prefix=$PREFIX --disable-nls
make
sudo make install
cd ..

# Build GCC (with C++ support)
cd gcc-11.2.0
./configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
sudo make install-gcc
cd ..

# Verify installation
$TARGET-gcc --version
$TARGET-g++ --version
```

## Building HELIOS

### 1. Clone the Repository

```bash
git clone https://github.com/PrestonTarlue/helios_os.git
cd helios_os
```

### 2. Build the Kernel

```bash
make build
```

This will compile:
- Bootloader (NASM → ELF64)
- Kernel C++ code
- Kernel assembly helpers
- File system module
- Shell module
- GUI module
- Browser module

### 3. Create Bootable ISO

```bash
make iso
```

Creates `build/helios.iso` bootable image.

### 4. Run in QEMU

```bash
make run-qemu
```

Boots HELIOS in QEMU emulator (256MB RAM, VGA display).

## Build Targets

```bash
make build      # Compile kernel and modules
make iso        # Create bootable ISO image
make run-qemu   # Run in QEMU emulator
make clean      # Remove build artifacts
make help       # Show build help
```

## Project Structure

```
helios_os/
├── bootloader/
│   └── boot.asm           # BIOS bootloader (16-bit → 32-bit)
├── kernel/
│   ├── kernel.cpp         # Main kernel code (C++)
│   └── kernel.asm         # Kernel assembly helpers
├── fs/
│   └── filesystem.cpp     # File system implementation
├── shell/
│   └── shell.cpp          # Command shell
├── gui/
│   └── gui.cpp            # GUI system
├── browser/
│   └── browser.cpp        # Web browser
├── Makefile               # Build system
├── linker.ld              # Linker script
├── grub.cfg               # GRUB bootloader config
├── README.md              # Project overview
└── BUILD.md               # This file
```

## Compiler Flags

### C/C++ Compilation
```
-ffreestanding            # No standard library
-fno-stack-protector      # Disable stack protection
-fno-builtin              # No built-in functions
-fno-exceptions           # C++ exceptions disabled
-fno-rtti                 # No runtime type info
-std=c++14                # C++14 standard
-Wall -Wextra             # Enable warnings
-O0 -g                    # Debug build
```

### Assembly
```
-f elf64                  # 64-bit ELF format
```

## Linker Script

The `linker.ld` file defines:
- Entry point: `kernel_entry`
- Load address: 0x100000 (1MB)
- Sections: .text, .rodata, .data, .bss

## Troubleshooting

### Build Errors

**Error: `x86_64-elf-gcc: command not found`**
- Install cross-compiler (see Prerequisites)
- Ensure it's in your PATH: `export PATH=/opt/cross/bin:$PATH`

**Error: `nasm: command not found`**
- Install NASM: `sudo apt-get install nasm`

**Error: `grub-mkrescue: command not found`**
- Install GRUB: `sudo apt-get install grub-pc-bin`

### ISO Creation

If `grub-mkrescue` fails, the Makefile falls back to `mkisofs`:
- Install: `sudo apt-get install mkisofs`

### QEMU Issues

**QEMU won't start:**
- Ensure QEMU is installed: `sudo apt-get install qemu-system-x86`
- Check CPU virtualization is enabled in BIOS

**Display issues:**
- Try different QEMU display options in Makefile
- Use `-display gtk`, `-display sdl`, or `-nographic`

## Development Workflow

```bash
# 1. Make changes to source code
vim kernel/kernel.cpp

# 2. Clean previous build
make clean

# 3. Rebuild
make build

# 4. Create ISO
make iso

# 5. Test in QEMU
make run-qemu

# 6. Debug if needed
# (QEMU will show output)

# 7. Commit changes
git add .
git commit -m "Implement new feature"
git push
```

## Performance Optimization

For optimized builds, change `BUILD_MODE` in Makefile:

```makefile
# Change from:
CFLAGS = -O0 -g
# To:
CFLAGS = -O2
```

## Debugging

### With GDB (if supported):
```bash
qemu-system-x86_64 -s -S -cdrom build/helios.iso
# In another terminal:
gdb
(gdb) target remote localhost:1234
(gdb) continue
```

### Serial Output:
Add to Makefile's QEMU command:
```bash
-serial stdio
```

## Real Hardware

### Creating Bootable USB
```bash
sudo dd if=build/helios.iso of=/dev/sdX bs=4M status=progress
sync
```

(Replace `/dev/sdX` with your USB device)

### Running on Real Hardware
1. Create bootable USB (see above)
2. Insert USB into target machine
3. Boot from USB (change boot order in BIOS)
4. HELIOS will load

## Clean Build

```bash
make clean && make build && make iso && make run-qemu
```

## Next Steps

1. ✅ Build and boot HELIOS
2. Test all shell commands
3. Verify GUI renders correctly
4. Test browser with built-in pages
5. Extend with new features

---

For detailed architecture information, see [ARCHITECTURE.md](ARCHITECTURE.md)
