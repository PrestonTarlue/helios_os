// HELIOS 1.0 Kernel
// Core kernel implementation in C++

extern "C" void kernel_main() {
    // Initialize kernel subsystems
    initialize_gdt();
    initialize_idt();
    initialize_memory();
    initialize_pic();
    initialize_pit();
    
    // Clear display
    clear_screen();
    
    // Print boot messages
    print_string("================================\n", 0x0F);
    print_string("HELIOS 1.0 Kernel Started\n", 0x0F);
    print_string("================================\n\n", 0x0F);
    
    print_string("Initializing system components...\n", 0x0F);
    
    // Initialize file system
    filesystem_init();
    print_string("[OK] File System initialized\n", 0x0A);
    
    // Initialize process management
    process_manager_init();
    print_string("[OK] Process Manager initialized\n", 0x0A);
    
    // Initialize GUI
    gui_init();
    print_string("[OK] GUI System initialized\n", 0x0A);
    
    // Initialize shell
    shell_init();
    print_string("[OK] Shell initialized\n\n", 0x0A);
    
    // Enable interrupts
    asm volatile("sti");
    
    print_string("Type 'help' for available commands.\n\n", 0x0F);
    
    // Start shell loop
    shell_run();
    
    // Should never reach here
    while(1) {
        asm volatile("hlt");
    }
}

// Video memory pointer (VGA text mode)
volatile unsigned char* video_memory = (unsigned char*) 0xB8000;
int cursor_x = 0;
int cursor_y = 0;

// Clear the screen
void clear_screen() {
    for (int i = 0; i < 80 * 25 * 2; i++) {
        video_memory[i] = 0;
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Print a single character
void print_char(char c, unsigned char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= 25) {
            cursor_y = 0;
        }
        return;
    }
    
    int offset = (cursor_y * 80 + cursor_x) * 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = color;
    
    cursor_x++;
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= 25) {
            cursor_y = 0;
        }
    }
}

// Print a string
void print_string(const char* str, unsigned char color) {
    while (*str) {
        print_char(*str++, color);
    }
}

// ============================================================================
// GDT (Global Descriptor Table) Setup
// ============================================================================

struct GDTEntry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct GDTPointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

GDTEntry gdt[3];
GDTPointer gdt_ptr;

void set_gdt_entry(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

extern "C" void gdt_flush(unsigned int);

void initialize_gdt() {
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 3) - 1;
    gdt_ptr.base = (unsigned int)&gdt;
    
    // Null descriptor
    set_gdt_entry(0, 0, 0, 0, 0);
    
    // Code descriptor (flat, readable, executable)
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    
    // Data descriptor (flat, writable)
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    gdt_flush((unsigned int)&gdt_ptr);
}

// ============================================================================
// IDT (Interrupt Descriptor Table) Setup
// ============================================================================

struct IDTEntry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct IDTPointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

IDTEntry idt[256];
IDTPointer idt_ptr;

extern "C" void idt_flush(unsigned int);

void set_idt_entry(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void initialize_idt() {
    idt_ptr.limit = sizeof(IDTEntry) * 256 - 1;
    idt_ptr.base = (unsigned int)&idt;
    
    // Initialize all IDT entries
    for (int i = 0; i < 256; i++) {
        set_idt_entry(i, 0, 0x08, 0);
    }
    
    idt_flush((unsigned int)&idt_ptr);
}

// ============================================================================
// Memory Management
// ============================================================================

void initialize_memory() {
    // Memory management initialization
    // To be expanded with paging support
}

// ============================================================================
// PIC (Programmable Interrupt Controller) Setup
// ============================================================================

void initialize_pic() {
    // PIC initialization
    // Port 0x20: Master PIC
    // Port 0xA0: Slave PIC
}

// ============================================================================
// PIT (Programmable Interval Timer) Setup
// ============================================================================

void initialize_pit() {
    // PIT initialization for timer interrupts
    // Default frequency: 18.2 Hz
    // Port 0x40-0x43
}

// ============================================================================
// Stub Functions (to be implemented)
// ============================================================================

void filesystem_init() {
    // File system initialization
}

void process_manager_init() {
    // Process manager initialization
}

void gui_init() {
    // GUI system initialization
}

void shell_init() {
    // Shell initialization
}

void shell_run() {
    print_string("User Core> ", 0x0F);
    // Shell command loop will be implemented here
}
