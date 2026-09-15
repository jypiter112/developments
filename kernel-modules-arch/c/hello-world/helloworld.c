#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Hello world.");

static int __init ModuleInit(void) {
  printk("Hello from ModuleInit!\n");
  return 0;
}

static void __exit ModuleExit(void) {
  printk("HelloWorld unloaded!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
