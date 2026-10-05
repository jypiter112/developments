#include "klib.h"
#include "jyp.h"
#include "keyboard.h"
#include "vga.h"
#include "ata.h"
#include <stdint.h>

void kgetline(char *buf, int max) {
  int len = 0;
  for (;;) {
    char c = kbd_getc();
    if (c == '\n') {
      vga_putc('\n', 0x0F);
      buf[len] = 0;
      return;
    } else if (c == '\b') {
      if (len > 0) {
        len--;
        vga_putc('\b', 0x0F);
      }
    } else if (len < max - 1) {
      buf[len++] = c;
      vga_putc(c, 0x0F);
    }
  }
}

// returns 1 when equal, 0 when not equal
int kstrcmpr(const char *s1, const char *s2, size_t strlen) {
  for (int i = 0; i < strlen; i++) {
    if (s1[i] != s2[i]) {
      return 0;
    }
  }
  return 1;
}
// returns the length of the string
int kstrlen(const char *s) {
  int len = 0;
  while (s[len] != 0) {
    len++;
  }
  return len;
}

// unsafe
void kprintf(const char* str){
  const uint8_t color = 0x0F; 
  vga_puts(str, color);
  vga_putc('\n', color);
}

// TODO: move to some input handler

void khandle_input(const char *input) {
  const int input_len = kstrlen(input);
  if (input_len == 0) {
    return;
  }
  // implement something better than this...
  if (kstrcmpr(input, "jyp", input_len)) {
    do_jyp();
  }
  if(kstrcmpr(input, "testread", input_len)){
    uint8_t disk1_sector0[512];
    ata_read(1, 0, 1, disk1_sector0);
    
    // null terminate just in case
    disk1_sector0[511] = 0;
    kprintf((const char*)disk1_sector0);
  }
  else {
    kprintf("Unknown command");
  }
}



// Memory management
// Move to mem.h in the future


/*
* Function: copies n bytes from src to dest
* src lib/klib.c
* 
* @param dest: destination pointer
* @param src: source pointer
* @param n: number of bytes to copy
* 
* @except: returns 0
*/
void *kmemcpy(void *dest, const void *src, size_t n){
  if(dest == 0 || src == 0 || n == 0){
    return 0;
  }

  char* d = (char*)dest;
  const char* s = (const char*)src;

  // forward copy
  for(size_t i = 0; i < n; i++){
    d[i] = s[i];
  }
  return dest;
}