[bits 16]
[org 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Clear screen (BIOS is still usable in real mode)
    mov ax, 0x0003
    int 0x10

    ; Enable A20 via the "fast A20" port
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Load the GDT and enter protected mode
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:pm_start       ; far jump: switches to 32-bit code

; ---------------- GDT ----------------
gdt_start:
    dq 0                        ; null descriptor (required)

gdt_code:
    dw 0xFFFF                   ; limit 0-15
    dw 0x0000                   ; base 0-15
    db 0x00                     ; base 16-23
    db 10011010b                ; present, ring 0, code, executable, readable
    db 11001111b                ; 4K granularity, 32-bit, limit 16-19
    db 0x00                     ; base 24-31

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b                ; present, ring 0, data, writable
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size
    dd gdt_start                ; address

CODE_SEG equ gdt_code - gdt_start   ; 0x08
DATA_SEG equ gdt_data - gdt_start   ; 0x10

; ---------------- 32-bit code ----------------
[bits 32]
pm_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000            ; stack in free memory

    ; Print directly to VGA text memory (no BIOS in protected mode)
    mov esi, msg
    mov edi, 0xB8000
    mov ah, 0x0F                ; white on black
.loop:
    lodsb
    test al, al
    jz .done
    mov [edi], ax               ; char in AL, attribute in AH
    add edi, 2
    jmp .loop
.done:
    hlt
    jmp .done

msg db "Now in 32-bit protected mode!", 0

times 510 - ($ - $$) db 0
dw 0xAA55
