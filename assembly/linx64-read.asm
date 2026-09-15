; Only works on linux!
section .data
    filename db "myfile", 0

section .bss    ; Used to reserve data
    output resb 15  

%macro exit 0
    mov rax, 60
    mov rdi, 0
    syscall
%endmacro

section .text
    global _start
_start:
    mov rax, 2          ; 2 = sys_open
    mov rdi, filename   ; targe file name
    mov rsi, 0          ; 0 = read only
    mov rdx, 0644o      ; o means octal value (file permissions)
    syscall             ; returns file descriptor of the file opened within RAX register

    push rax            ; save fd for closing op
    mov rdi, rax        ; move fd to rdi
    mov rax, 0          ; SYS_READ
    mov rsi, output     ; store value where
    mov rdx, 15         ; # bytes to read
    syscall

    mov rax, 3          ; close file
    pop rdi         
    syscall

    mov rax, 1          ; Print text
    mov rdi, 1          ; Print here? 1 = here
    mov rsi, output     ; Print what
    mov rdx, 15
    syscall			    ; Do the sys call
    exit
