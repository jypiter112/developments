#include "keyboard.h"
#include "io.h"

#define KBD_DATA 0x60
#define KBD_STATUS 0x64
#define KBD_OUTBUF_FULL 0x01

#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_CAPS 0x3A

static const char keymap[] = "\0\033"
                             "1234567890-=\b\t"
                             "qwertyuiop[]\n\0"
                             "asdfghjkl;'`\0\\"
                             "zxcvbnm,./\0*\0 ";

static const char keymap_shift[] = "\0\033"
                                   "!@#$%^&*()_+\b\t"
                                   "QWERTYUIOP{}\n\0"
                                   "ASDFGHJKL:\"~\0|"
                                   "ZXCVBNM<>?\0*\0 ";

static int shift_down;
static int caps_on;
static int extended;

char kbd_poll(void) {
  if (!(inb(KBD_STATUS) & KBD_OUTBUF_FULL))
    return 0;

  uint8_t sc = inb(KBD_DATA);

  if (sc == 0xE0) {
    extended = 1;
    return 0;
  }
  if (extended) {
    extended = 0;
    return 0;
  }

  if (sc & 0x80) {
    sc &= 0x7F;
    if (sc == SC_LSHIFT || sc == SC_RSHIFT)
      shift_down = 0;
    return 0;
  }

  if (sc == SC_LSHIFT || sc == SC_RSHIFT) {
    shift_down = 1;
    return 0;
  }
  if (sc == SC_CAPS) {
    caps_on ^= 1;
    return 0;
  }

  if (sc >= sizeof(keymap) - 1)
    return 0;

  char c = shift_down ? keymap_shift[sc] : keymap[sc];

  if (caps_on) {
    if (c >= 'a' && c <= 'z')
      c -= 32;
    else if (c >= 'A' && c <= 'Z')
      c += 32;
  }
  return c;
}

char kbd_getc(void) {
  char c;
  while ((c = kbd_poll()) == 0)
    __asm__ volatile("pause");
  return c;
}
