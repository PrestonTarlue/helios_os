/* HELIOS 1.0 Linker Script */

ENTRY(kernel_entry)

SECTIONS
{
    . = 0x100000;  /* Load address: 1MB */
    
    .text : {
        *(.text)
    }
    
    .rodata : {
        *(.rodata)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        *(COMMON)
        *(.bss)
    }
}
