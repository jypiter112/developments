#include "klib.h"
#include "vga.h"

void kmain(void) {
  char line[64];
  
  vga_clear();
  vga_puts("Hello from jypiter 112 kernel!\n", 0x07);
  vga_puts("Waiting input...\n\n", 0x07);

  for (;;) {
    vga_puts("> ", 0x0E);
    kgetline(line, sizeof(line));
    khandle_input(line);
  }
}
