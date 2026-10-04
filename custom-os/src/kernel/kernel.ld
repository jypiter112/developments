ENTRY(_start)

SECTIONS {
    . = 0x1000;

    .text   : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }
    .bss    : { __bss_start = .;
                *(COMMON) *(.bss*)
                __bss_end = .; 
              }

    /DISCARD/ : { *(.eh_frame*) *(.note*) *(.comment) }
}
