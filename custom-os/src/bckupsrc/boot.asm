[bits 16]
[org 0x7C00]            ; BIOS loads the boot sector here

start:
    cli
    xor ax, ax          ; zero the segment registers
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      ; stack grows down from just below the bootloader
    sti

    mov si, msg
    call print

hang:
    hlt
    jmp hang

; Print null-terminated string at DS:SI using BIOS teletype
print:
    mov ah, 0x0E        ; INT 10h, function 0Eh: teletype output
.loop:
    lodsb               ; load byte at [SI] into AL, increment SI
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

msg db "Hello from my bootloader!", 0

times 510 - ($ - $$) db 0   ; pad to 510 bytes
dw 0xAA55                   ; boot signature (bytes 55 AA)
