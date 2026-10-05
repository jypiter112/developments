#ifndef IO_H
#define IO_H
#include <stdint.h>

// helper functions to read form ports

static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}
// read a word from port to memory
static inline void insw(uint16_t port, void *addr, uint32_t count){
  __asm__ volatile("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}
static inline void outsw(uint16_t port, const void *addr, uint32_t count){
  __asm__ volatile("rep outsw": "+S"(addr), "+c"(count) : "d"(port) : "memory");
}

#endif // IO_H
