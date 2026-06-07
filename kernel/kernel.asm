; HELIOS 1.0 Kernel Assembly Support
; x86-64 assembly functions for kernel

[BITS 32]

; Kernel entry point
[GLOBAL kernel_entry]
[EXTERN kernel_main]

kernel_entry:
    ; Setup stack
    mov esp, 0x30000
    
    ; Call C++ kernel main function
    call kernel_main
    
    ; Should never reach here
    cli
    hlt
    jmp $

; GDT flush function
; Loads GDT and reloads segment registers
[GLOBAL gdt_flush]
gdt_flush:
    mov eax, [esp+4]        ; Get GDT pointer from stack
    lgdt [eax]              ; Load GDT
    
    ; Reload segment registers
    mov ax, 0x10            ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Far jump to reload code segment
    jmp 0x08:flush_cs
flush_cs:
    ret

; IDT flush function
; Loads IDT (Interrupt Descriptor Table)
[GLOBAL idt_flush]
idt_flush:
    mov eax, [esp+4]        ; Get IDT pointer from stack
    lidt [eax]              ; Load IDT
    ret

; ISR (Interrupt Service Routine) stub
[GLOBAL isr0]
isr0:
    cli
    push byte 0             ; No error code
    push byte 0             ; Interrupt number
    jmp isr_common_stub

; ISR common stub
isr_common_stub:
    pusha                   ; Push all general purpose registers
    
    ; Push data segment
    mov ax, ds
    push eax
    
    ; Load kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call ISR handler (implemented in C++)
    call isr_handler
    
    ; Pop data segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Pop general purpose registers
    popa
    
    ; Remove error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret

[EXTERN isr_handler]
