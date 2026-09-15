#include "asm-generic/fcntl.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Hello world.");

static void ___change_brightness(char level[]) { 
  // Change Brightness
  struct file *f;
  loff_t pos = 0;
  f = filp_open("/sys/class/leds/platform::kbd_backlight/brightness",
      O_WRONLY,
      0);
  if(!IS_ERR(f)) {
    kernel_write(f, level, sizeof(level) - 1, &pos);
    filp_close(f, 0);
  }
}
static int __init ModuleInit(void) {
  printk("Brightness module loaded.\n");

  return 0;
}

static void __exit ModuleExit(void) {
  printk("Brightness module unloaded.\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
