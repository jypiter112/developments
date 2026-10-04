#include "vga.h"
#include "io.h"

#define VGA_MEM ((volatile uint16_t *)0xB8000)
#define VGA_COLS 80
#define VGA_ROWS 25

static int cursor;

static void update_cursor(void) {
  outb(0x3D4, 14);
  outb(0x3D5, (cursor >> 8) & 0xFF);
  outb(0x3D4, 15);
  outb(0x3D5, cursor & 0xFF);
}

static void scroll(void) {
  for (int i = 0; i < VGA_COLS * (VGA_ROWS - 1); i++)
    VGA_MEM[i] = VGA_MEM[i + VGA_COLS];
  for (int i = VGA_COLS * (VGA_ROWS - 1); i < VGA_COLS * VGA_ROWS; i++)
    VGA_MEM[i] = (0x07 << 8) | ' ';
  cursor -= VGA_COLS;
}

void vga_clear(void) {
  for (int i = 0; i < VGA_COLS * VGA_ROWS; i++)
    VGA_MEM[i] = (0x07 << 8) | ' ';
  cursor = 0;
  update_cursor();
}

void vga_putc(char c, uint8_t color) {
  if (c == '\n') {
    cursor = (cursor / VGA_COLS + 1) * VGA_COLS;
  } else if (c == '\b') {
    if (cursor > 0) {
      cursor--;
      VGA_MEM[cursor] = (0x07 << 8) | ' ';
    }
  } else if (c == '\t') {
    cursor = (cursor / 8 + 1) * 8;
  } else {
    VGA_MEM[cursor++] = (color << 8) | (uint8_t)c;
  }
  if (cursor >= VGA_COLS * VGA_ROWS)
    scroll();
  update_cursor();
}

void vga_puts(const char *s, uint8_t color) {
  while (*s)
    vga_putc(*s++, color);
}
