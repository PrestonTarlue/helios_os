# HELIOS 1.0

A modern 64-bit operating system built from scratch in C++ for x86-64 architecture.

## Features

- ✅ Bootloader (BIOS/UEFI support)
- ✅ Protected mode kernel with GDT/IDT
- ✅ Memory management with paging support
- ✅ Process management and scheduling
- ✅ File system (ext2-like)
- ✅ Shell with Linux-compatible commands
- ✅ GUI system with glass effects and wallpaper
- ✅ Built-in web browser
- ✅ Interrupt handling (PIC/PIT)
- ✅ Multi-tasking support

## Quick Start

### Build
```bash
make build
make iso
```

### Run in QEMU
```bash
make run-qemu
```

## System Requirements

- Cross-compiler: x86_64-elf-gcc/g++
- Assembler: NASM
- Tools: GRUB, QEMU, Make
- RAM: 256MB minimum
- Disk: 1GB

## Project Structure

```
helios_os/
├── bootloader/      # x86-64 BIOS bootloader
├── kernel/          # Core kernel (C++ + assembly)
├── fs/              # File system implementation
├── shell/           # Command shell
├── gui/             # GUI system with glass effects
├── browser/         # Built-in web browser
├── Makefile         # Build system
├── linker.ld        # Linker script
├── grub.cfg         # GRUB configuration
└── BUILD.md         # Build instructions
```

## Shell Commands

```
echo <text>      - Print text
help             - Show help
clear            - Clear screen
ls               - List files
cd <dir>         - Change directory
mkdir <name>     - Create directory
cat <file>       - Display file
rm <file>        - Remove file
pwd              - Print working directory
about            - About HELIOS
browser          - Launch browser
reboot           - Reboot system
shutdown         - Shutdown system
```

## Architecture

- **CPU**: x86-64 (Intel/AMD)
- **Boot**: BIOS + GRUB
- **Memory**: Paging enabled, 4KB pages
- **Scheduling**: Round-robin, 10ms quantum
- **File System**: Inode-based

## Build Instructions

See [BUILD.md](BUILD.md) for detailed build setup.

## GUI Features

- Glass effect panels
- Gradient wallpaper
- Semi-transparent taskbar
- Window rendering support
- 1024x768 resolution
- 32-bit ARGB color depth

## Browser Features

- Page navigation
- History support
- Built-in URLs:
  - `about:home` - Home page
  - `about:about` - About HELIOS
  - `about:version` - Version info

## Future Enhancements

- [ ] Virtual memory / demand paging
- [ ] Preemptive multitasking
- [ ] System calls interface
- [ ] Dynamic memory allocation
- [ ] Advanced file system
- [ ] Network stack (TCP/IP)
- [ ] User/kernel mode separation
- [ ] IPC (Inter-Process Communication)
- [ ] Security features (access control)
- [ ] File permissions

## Development

To modify HELIOS:

1. Edit source files in respective directories
2. Run `make clean` to remove old builds
3. Run `make build` to recompile
4. Run `make iso` to create bootable image
5. Run `make run-qemu` to test

## License

HELIOS 1.0 - Built from scratch with passion

## Author

Preston Tarlue

---

**HELIOS: High-Performance Embedded Linux-Inspired Operating System**

Built with C++ and Assembly 🚀
