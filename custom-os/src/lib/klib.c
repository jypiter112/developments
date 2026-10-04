#include "klib.h"
#include "jyp.h"
#include "keyboard.h"
#include "vga.h"

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
int kstrcmpr(const char *s1, const char *s2, int strlen) {
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

void khandle_input(const char *input) {
  const int input_len = kstrlen(input);
  if (input_len == 0) {
    return;
  }

  if (kstrcmpr(input, "jyp", input_len)) {
    do_jyp();
  } else {
    // VGA echo
    vga_puts("Unknown command: ", 0x0C);
    vga_puts(input, 0x0F);
    vga_putc('\n', 0x0F);
  }
}
