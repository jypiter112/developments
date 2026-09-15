; Works only on x32 supported elf (linux)
; Compile with: $ nasm -f elf32 -o hello.o hello.asm
; Link with: $ ld -m elf_i386 -o hello hello.o
  section .data
        msg db 'Hello world!', 0
        len equ $ -msg
    section .text
    global _start
        _start:
        mov edx, len
        mov ecx, msg
        mov ebx, 1

        mov eax, 4      ; Syscall write
        int 0x80

        mov eax, 1      ; Syscall exit
        int 0x80
