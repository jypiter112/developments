/*
  this is good if you really really hate using stdlib for some obscure reason
  compilation is with clang:
  $  clang -nostdlib -static -o hello64 sc_64.c
*/
typedef unsigned long long uint64_t;
void _scwrite(const char* msg, uint64_t len){
    asm volatile(
        "mov $1, %%rax\n\t"
        "mov $1, %%rdi\n\t"
        "mov %0, %%rsi\n\t"
        "mov %1, %%rdx\n\t"
        "syscall"
        :
        : "r"(msg), "r"(len)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
}
__attribute__((noreturn)) void _scexit(uint64_t code) {
    asm volatile(
        "mov $60, %%rax\n\t"
        "mov %0, %%rdi\n\t"
        "syscall"
        :
        : "r"(code)
        : "%rax", "%rdi"
    );
    __builtin_unreachable();
}

void _start(){
    const char* msg = "Hello, World!\n";
    _scwrite(msg, 14);
    _scexit(0);
}
