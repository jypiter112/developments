#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Hello world.");

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
static int my_get(char *buff, const struct kernel_param *kp){
  // kp->arg points to (int)num in this case
  int val = *(int *)kp->arg;
  return sprintf(buff, "%d\n", val);
}

// c class basically
static const struct kernel_param_ops param_ops = {
  .set = my_set,
  .get = my_get,
};

// simpcb is the param name here
static int num;
module_param_cb(simpcb, &param_ops, &num, 0664);

static int __init ModuleInit(void) {
  printk(KERN_WARNING "hello ... num=%d\n", num);
	return 0;
}

static void __exit ModuleExit(void) {
  printk(KERN_WARNING "hello ... num=%d\n", num);
}

module_init(ModuleInit);
module_exit(ModuleExit);
