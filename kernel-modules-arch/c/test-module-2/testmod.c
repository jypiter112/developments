#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Hello world.");

// Params
static int irq = 10;
module_param(irq, int, 0660);

static int debug = 0;
module_param(debug, int, 0664);

static char *devname = "simpdev";
module_param(devname, charp, 0660);

// module_param_cb test
static int my_set(const char *val, const struct kernel_param *kp){
  int n = 0, ret;
  
  ret = kstrtoint(val, 10, &n);
  // Custom bound check
  if(ret != 0 || n < 1 || n > 32)
    return -EINVAL;
  // set the value
  return param_set_int(val, kp);
}

// c class basically
static const struct kernel_param_ops param_ops = {
  .set = my_set,
  .get = param_get_int,
};

static int num;
module_param_cb(simpcb, &param_ops, &num, 0664);

static int __init ModuleInit(void) {
  printk(KERN_WARNING "hello... irq=%d name=%s debug=%d\n",irq,devname,debug);
	return 0;
}

static void __exit ModuleExit(void) {
  printk(KERN_WARNING "bye... irq=%d name=%s debug=%d\n",irq,devname,debug);
}

module_init(ModuleInit);
module_exit(ModuleExit);
