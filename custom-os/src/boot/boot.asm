[bits 16]
[org 0x7C00]

KERNEL_ADDR     equ 0x1000
KERNEL_SECTORS  equ 20              ; 10 KB, raise if your kernel grows

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [boot_drive], dl            ; BIOS passes boot drive in DL

    ; clear screen
    mov ax, 0x0003
    int 0x10

    mov si, msg_load
    call print

    ; ---- read kernel from disk (sector 2 onward) to 0000:1000 ----
    mov bx, KERNEL_ADDR             ; ES:BX = destination
    mov ah, 0x02                    ; read sectors
    mov al, KERNEL_SECTORS
    mov ch, 0                       ; cylinder 0
    mov cl, 2                       ; sector 2 (sector 1 is this boot sector)
    mov dh, 0                       ; head 0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; ---- enter protected mode ----
    cli
    in al, 0x92                     ; fast A20
    or al, 2
    out 0x92, al

    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:pm_start

disk_error:
    mov si, msg_err
    call print
.hang:
    hlt
    jmp .hang

print:                              ; print string at DS:SI
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

boot_drive  db 0
msg_load    db "Loading kernel...", 13, 10, 0
msg_err     db "Disk read error!", 0

; ---------------- GDT ----------------
gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF, 0x0000
    db 0x00, 10011010b, 11001111b, 0x00
gdt_data:
    dw 0xFFFF, 0x0000
    db 0x00, 10010010b, 11001111b, 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 32]
pm_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp KERNEL_ADDR                 ; hand over to the kernel

times 510 - ($ - $$) db 0
dw 0xAA55
