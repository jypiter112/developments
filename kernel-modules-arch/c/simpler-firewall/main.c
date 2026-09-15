// Must have includes
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>    // kmalloc etc
#include <linux/uaccess.h> // copy_to_user copy_from_user

// must have
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Simple Linux device driver (Real Linux Device Driver)");
MODULE_VERSION("1.0");

// Entry, exit
static int __init my_init(void) {
  pr_info("Module initialized\n");
  return 0;
}
static void __exit my_exit(void) { pr_info("Moduel exited\n"); }

module_init(my_init);
module_exit(my_exit);
