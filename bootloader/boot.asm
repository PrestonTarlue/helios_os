; HELIOS 1.0 Bootloader
; x86-64 BIOS Bootloader
; Loads kernel and enters protected mode

[BITS 16]
[ORG 0x7C00]

; Boot sector starts here
boot_start:
    jmp short start_boot
    nop

; BIOS Parameter Block (FAT12)
oem_name:           db "HELIOS1 "
bytes_per_sector:   dw 512
sectors_per_cluster: db 1
reserved_sectors:   dw 1
num_fats:           db 2
num_root_entries:   dw 224
total_sectors_small: dw 2880
media_descriptor:   db 0xF0
sectors_per_fat:    dw 9
sectors_per_track:  dw 18
num_heads:          dw 2
hidden_sectors:     dd 0
total_sectors_large: dd 0

; Extended boot record
drive_number:       db 0
reserved:           db 0
boot_signature:     db 0x29
volume_id:          dd 0x12345678
volume_label:       db "HELIOS     "
file_system_type:   db "FAT12   "

start_boot:
    ; Initialize segment registers
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; Set video mode to 80x25 text
    mov ax, 0x0003
    int 0x10
    
    ; Display boot message
    mov si, boot_msg
    call print_string
    
    ; Load kernel from disk
    ; Read 10 sectors starting from sector 2
    mov ax, 0x1000          ; Destination segment
    mov es, ax
    xor bx, bx              ; Destination offset
    mov ah, 0x02            ; Read sectors function
    mov al, 10              ; Number of sectors to read
    mov ch, 0               ; Cylinder 0
    mov cl, 2               ; Sector 2
    mov dh, 0               ; Head 0
    mov dl, 0x80            ; Drive 0x80 (first HDD)
    int 0x13                ; BIOS disk read
    
    ; Check for disk read error
    jc disk_error
    
    ; Display loaded message
    mov si, kernel_loaded_msg
    call print_string
    
    ; Enter protected mode
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT
    
    ; Set PE (Protected Enable) bit in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far jump to protected mode code
    jmp 0x08:protected_mode

; Print string function (real mode)
print_string:
    mov ah, 0x0E            ; TTY output function
.loop:
    lodsb                   ; Load byte from [DS:SI] into AL
    cmp al, 0               ; Check for null terminator
    je .done
    int 0x10                ; BIOS video interrupt
    jmp .loop
.done:
    ret

; Disk error handler
disk_error:
    mov si, error_msg
    call print_string
    jmp $                   ; Infinite loop

; Messages
boot_msg:           db "HELIOS 1.0 Bootloader", 13, 10, 0
kernel_loaded_msg:  db "Kernel loaded - Entering protected mode", 13, 10, 0
error_msg:          db "Disk read error!", 13, 10, 0

; Global Descriptor Table (GDT)
align 8
gdt_start:
    ; Null descriptor (required)
    dq 0x0000000000000000
    
    ; Code descriptor (flat 32-bit)
    dq 0x00209A0000000FFF
    
    ; Data descriptor (flat 32-bit)
    dq 0x0020920000000FFF
    
gdt_end:

; GDT Descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1     ; Size
    dd gdt_start                    ; Address

; Protected mode code (32-bit)
[BITS 32]
protected_mode:
    ; Setup segment registers for protected mode
    mov ax, 0x10                ; Data segment selector (gdt entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Setup stack for kernel
    mov esp, 0x20000
    
    ; Jump to kernel entry point
    ; Kernel is loaded at 0x1000 (linear address 0x10000)
    jmp 0x10000

; Boot sector padding
times 510 - ($ - $$) db 0

; Boot signature
dw 0xAA55
