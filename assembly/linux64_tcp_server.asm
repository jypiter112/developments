section .bss
    buf resb 1024
section .data
    msg db 'Hello World!', 0xA
    len equ $ - msg
addr:               ; sockaddr_in (16 bytes)
    dw 2            ; AF_INET
    dw 0x901F      ; htons(8080)
    dd 0x00000000   ; INADDR_ANY
    times 8 db 0    ; padding
section .text
    global _start

strlen:
    xor rax, rax       ; length to 0
.loop:
    mov al, byte [rdi + rax] ; load byte at s[length]
    cmp al, 0               ; compare with null terminator
    je .done                ; if zero, done
    inc rax                 ; length++
    jmp .loop
.done:
    ret
 
print:
    cmp rax, 0          ; check that legnth isnt zero
    je accept_loop
    mov rdi, 1         ; stdout
    mov rsi, buf       ; buffer
    mov rdx, rax       ; length
    mov rax, 1         ; syscall: write
    syscall
    ret                ; safe return
exit:
    mov rax, 60        ; syscall: exit
    xor rdi, rdi       ; status 0
    syscall            ; exits process (never returns)

; Networking
socket:
    mov rax, 41     ; syscall: socket
    mov rdi, 2      ; family, AF_INET=2
    mov rsi, 1      ; type SOCK_STREAM=1
    mov rdx, 0      ; protocol 0
    syscall
    ret
bind:
    mov rax, 49     ; syscall: bind
    mov rdi, r8     ; fd
    mov rsi, addr   ; addr
    mov rdx, 16     ; len (addr) = 16 bytes
    syscall
    ret
listen:
    mov rax, 50
    mov rdi, r8     ; already is here ;
    mov rsi, 1      ; backlog
    syscall
    ret
accept:
    mov rax, 43     ; syscall accept
    mov rdi, r8     ; already here... ;
    mov rsi, 0      
    mov rdx, 0
    syscall
    ret
zerobuf:
    mov rcx, 1024
    lea rdi, [rel buf]
    xor al, al
    rep stosb   ; fill with zeroes
    ret
read:
    call zerobuf
    mov rax, 0
    mov rdi, r9         ; client fd
    lea rsi, [rel buf]  ; pointer to buffer
    mov rdx, 1024       ; num to read
    syscall
    ret
closeclient:
    mov rax, 3
    mov rdi, r9
    syscall
    ret
closeall:
    mov rax, 3      ;syscall for close
    mov rdi, r9     ; client fd
    syscall

    mov rdi, r8     ; sock fd
    syscall
    ret
_start:
    call socket
    mov r8, rax    ; sock fd

    call bind
    call listen

accept_loop:
    call accept
    mov r9, rax     ; client fd -> r9

.read_loop:
    call read
    cmp rax, 0
    je .close_client
    call print      ; rax has length of buf
    jmp .read_loop
.close_client:
    call closeclient
    jmp accept_loop   ; loop forever

    call closeall

    call exit      ; safe call to exit
