# HELIOS 1.0 - System Architecture

## Overview

HELIOS is a 64-bit monolithic kernel operating system built from scratch for x86-64 architecture. It implements core OS functionality in C++ with minimal assembly.

## Boot Sequence

### Stage 1: Real Mode (16-bit)
```
0x0000 ──→ BIOS loads bootloader at 0x7C00
           ↓
           Bootloader identifies boot device
           ↓
           Reads disk sectors into memory
           ↓
           Sets up GDT (Global Descriptor Table)
           ↓
           Transitions to protected mode
```

### Stage 2: Protected Mode (32-bit)
```
0x7C00 ─��→ Bootloader jumps to 0x10000
           ↓
           Kernel initialization begins
           ↓
           Sets up stack (0x20000)
           ↓
           Initializes GDT properly
           ↓
           Initializes IDT
           ↓
           Enables paging (if supported)
```

### Stage 3: Kernel Startup (32-bit)
```
0x10000 ──→ kernel_main() called
            ↓
            Initialize GDT
            Initialize IDT
            Initialize memory management
            Initialize PIC (Programmable Interrupt Controller)
            Initialize PIT (Programmable Interval Timer)
            Enable interrupts (STI)
            ↓
            Initialize subsystems:
            - Filesystem
            - Process manager
            - GUI system
            - Shell
            ↓
            Enter shell loop
```

## Memory Layout

### Physical Memory Map

```
0xFFFFFFFF ┌────────────────────────┐
           │  Kernel Space          │
           │  (Reserved)            │
           ├────────────────────────┤
0xE0000000 │  Linear Framebuffer    │
           │  (GUI Display)         │
           │  1024x768x32bpp        │
           ├────────────────────────┤
0xD0000000 │  Heap                  │
           │  (Dynamic Memory)      │
           │  Growing Upward        │
           ├────────────────────────┤
0x00030000 │  Stack                 │
           │  (Growing Downward)    │
           ├────────────────────────┤
0x00010000 │  Kernel Code + Data    │
           │  (0x100000 from link)  │
           ├────────────────────────┤
0x00001000 │  Page Tables           │
           │  (Paging info)         │
           ├────────────────────────┤
0x00000600 │  BIOS Data             │
           ├────────────────────────┤
0x00000400 │  IVT (if real mode)    │
           ├────────────────────────┤
0x00000000 │  Null/Unusable         │
           └────────────────────────┘
```

## Core Components

### 1. Bootloader (`bootloader/boot.asm`)

**Responsibilities:**
- Load kernel from disk
- Set up minimal GDT
- Transition to protected mode
- Jump to kernel entry point

**Key Structures:**
```
BootSector Layout:
- Boot code (446 bytes)
- Partition table (64 bytes)
- Boot signature (2 bytes)
```

**GDT Setup:**
```
Descriptor 0: Null (required)
Descriptor 1: Code (0x00209A00)
Descriptor 2: Data (0x00209200)
```

### 2. Kernel (`kernel/kernel.cpp`)

**Main Function:** `kernel_main()`

**Key Subsystems:**
- GDT Management
- IDT Management  
- Memory Management
- Interrupt Handling
- Video I/O

**GDT (Global Descriptor Table):**
```cpp
struct GDTEntry {
    unsigned short limit_low;     // Bits 0-15 of limit
    unsigned short base_low;      // Bits 0-15 of base
    unsigned char base_middle;    // Bits 16-23 of base
    unsigned char access;         // Access byte
    unsigned char granularity;    // Granularity + high limit
    unsigned char base_high;      // Bits 24-31 of base
};
```

**IDT (Interrupt Descriptor Table):**
```cpp
struct IDTEntry {
    unsigned short base_lo;       // Low 16 bits of ISR
    unsigned short sel;           // Code segment selector
    unsigned char always0;        // Reserved (always 0)
    unsigned char flags;          // Access/type flags
    unsigned short base_hi;       // High 16 bits of ISR
};
```

**Video I/O:**
- Base address: 0xB8000 (VGA text mode)
- Resolution: 80x25 text
- Format: 2 bytes per character (char + color)
- Color format: High nibble = background, low nibble = foreground

### 3. Interrupt Handling

**Interrupt Vector Table (IVT):**
```
0x00-0x1F: CPU Exceptions
  0x00: Divide by Zero
  0x08: Double Fault
  0x0D: General Protection Fault
  0x0E: Page Fault

0x20-0x2F: PIC Interrupts (IRQ 0-15)
  0x20: Timer (IRQ 0)
  0x21: Keyboard (IRQ 1)
  0x28: Disk (IRQ 8)

0x30-0xFF: Software Interrupts
```

**PIC (Programmable Interrupt Controller):**
- Master: Port 0x20/0x21
- Slave: Port 0xA0/0xA1
- Handles external hardware interrupts

**PIT (Programmable Interval Timer):**
- Port: 0x40-0x43
- Default frequency: 18.2 Hz
- Used for preemptive multitasking

### 4. File System (`fs/filesystem.cpp`)

**Inode Structure:**
```cpp
struct Inode {
    unsigned int inode_number;    // Unique inode ID
    unsigned int size;            // File size in bytes
    unsigned int permissions;     // Unix-style permissions
    unsigned int created_time;    // Creation timestamp
    unsigned char type;           // File type (regular, dir, etc)
    char name[256];               // Filename
};
```

**Operations:**
- `fs_create_inode()` - Create new inode
- `fs_write_file()` - Write file to disk
- `fs_read_file()` - Read file from disk
- `fs_delete_file()` - Delete file
- `fs_create_directory()` - Create directory
- `fs_list_directory()` - List directory contents

### 5. Shell (`shell/shell.cpp`)

**Command Structure:**
```cpp
class Command {
    virtual void execute(const char* args) = 0;
};
```

**Implemented Commands:**
- `echo` - Print text
- `help` - Display help
- `clear` - Clear screen
- `ls` - List files
- `cd` - Change directory
- `mkdir` - Create directory
- `cat` - Display file contents
- `rm` - Remove file
- `pwd` - Print working directory
- `about` - About HELIOS
- `browser` - Launch browser

**Prompt:** `User Core> `

### 6. GUI System (`gui/gui.cpp`)

**Graphics Mode:**
- Resolution: 1024x768 pixels
- Color depth: 32-bit ARGB
- Framebuffer: 0xE0000000
- Size: ~3MB

**Components:**
- Wallpaper (gradient)
- Glass effect panels
- Taskbar
- Window system
- Icons

**Color Scheme:**
- Background: Dark blue gradient
- Glass: Semi-transparent white
- Text: White
- Accent: Light blue

### 7. Browser (`browser/browser.cpp`)

**Features:**
- HTML rendering (basic)
- Page navigation
- History management
- Built-in pages

**Built-in URLs:**
```
about:home       - Home page with links
about:about      - OS information
about:version    - Version details
```

**HTML Parser:** Simple string matching for tags

## Process Management

### Process Control Block (PCB)
```cpp
struct PCB {
    unsigned int pid;             // Process ID
    unsigned int state;           // Running/Ready/Blocked
    unsigned int priority;        // Priority level
    unsigned int cpu_time;        // Milliseconds used
    void* stack_pointer;          // Current stack pointer
    void* instruction_pointer;    // Current IP
    unsigned char* memory;        // Process memory
};
```

### Scheduling
- **Algorithm:** Round-robin
- **Time Quantum:** 10ms (via PIT)
- **Context Switch:** On timer interrupt

## Device Drivers

### Keyboard Driver
- Interrupt: IRQ 1 (0x21)
- Scancode → ASCII conversion
- Input buffering

### Disk Driver
- Interrupt: IRQ 8 (0x28) [hard disk]
- BIOS INT 0x13 calls
- Sector-based I/O

### Display Driver
- VGA text mode (0xB8000)
- Linear framebuffer (0xE0000000)
- BIOS video services

## System Calls (Future)

**Process Management:**
```cpp
sys_fork()        // Create process
sys_exec()        // Execute program
sys_exit()        // Exit process
sys_wait()        // Wait for child
```

**File I/O:**
```cpp
sys_open()        // Open file
sys_read()        // Read file
sys_write()       // Write file
sys_close()       // Close file
```

**Memory:**
```cpp
sys_mmap()        // Map memory
sys_munmap()      // Unmap memory
```

## Hardware Requirements

| Component | Requirement |
|-----------|-------------|
| **CPU** | x86-64 compatible (Intel/AMD) |
| **RAM** | 256MB minimum, 512MB recommended |
| **Storage** | 1GB disk space |
| **Display** | VGA (80x25 text) or framebuffer capable |
| **Input** | PS/2 keyboard |

## Software Architecture

```
┌─────────────────────────────────┐
│  User Applications              │
│  (Shell, Browser, GUI)          │
├─────────────────────────────────┤
│  System Libraries               │
│  (FileSystem, Memory, Process)  │
├─────────────────────────────────┤
│  Kernel Services                │
│  (Interrupts, Scheduling)       │
├─────────────────────────────────┤
│  Hardware Abstraction Layer     │
│  (Drivers, Device I/O)          │
├─────────────────────────────────┤
│  Hardware (CPU, RAM, Disk)      │
└─────────────────────────────────┘
```

## Build Process

```
Source Files
    ↓
    ├─→ boot.asm ──(NASM)──→ boot.o
    ├─→ kernel.cpp ──(G++)──→ kernel.o
    ├─→ kernel.asm ──(NASM)──→ kernel_asm.o
    ├─→ filesystem.cpp ──(G++)──→ filesystem.o
    ├─→ shell.cpp ──(G++)──→ shell.o
    ├─→ gui.cpp ──(G++)──→ gui.o
    └─→ browser.cpp ──(G++)──→ browser.o
    ↓
    Linker (linker.ld)
    ↓
    kernel.elf
    ↓
    GRUB ISO Creator
    ↓
    helios.iso
```

## Performance Considerations

- **Kernel Size:** ~50KB
- **Boot Time:** ~2 seconds
- **Shell Response:** <100ms
- **Context Switch:** <1ms
- **Memory Usage:** ~10MB typical

## Future Enhancements

1. **Virtual Memory**
   - Demand paging
   - Page replacement
   - Memory protection

2. **Advanced Scheduling**
   - Priority levels
   - Real-time support
   - Load balancing

3. **IPC (Inter-Process Communication)**
   - Pipes
   - Message queues
   - Shared memory

4. **Network Stack**
   - TCP/IP implementation
   - Ethernet driver
   - Socket API

5. **Security**
   - User/kernel separation
   - Access control lists
   - Privilege levels

---

**HELIOS Architecture Version 1.0**

Built with precision and passion 🚀
