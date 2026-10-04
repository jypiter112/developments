#include "jyp.h"
#include "vga.h"

void jyp_init(void) {}

void do_jyp(void) { vga_puts("jyp\n", 0x0A); }
