#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>

// IOCTL Example
#define MY_MAGIC 'M'

#define WR_VALUE _IOW(MY_MAGIC, 1, int)
#define RD_VALUE _IOR(MY_MAGIC, 2, int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Hello world.");

// value set
static int value = 0;

// custom ioctl device
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  int tmp;
  switch(cmd){
    case WR_VALUE:
      if(copy_from_user(&tmp, (int __user *)arg, sizeof(tmp)))
        return -EFAULT;
      value = tmp;
      printk(KERN_INFO "<value> set to %d\n", value);
      break;
    case RD_VALUE:
      if(copy_to_user((int __user *)arg, &value, sizeof(value)))
        return -EFAULT;
      break;
    default:
      return -EINVAL;
  }
  return 0;
}
// file operations
static struct file_operations fops = {
  .owner = THIS_MODULE,
  .unlocked_ioctl = my_ioctl,
};
static int major;

static int __init ModuleInit(void) {
  major = register_chrdev(0, "mydev", &fops);
  if(major < 0)
    return major;
  printk(KERN_WARNING "hello from ioctl device\n");
  printk(KERN_WARNING "mydev registered with major %d\n", major);
	return 0;
}

static void __exit ModuleExit(void) {
  unregister_chrdev(major, "mydev");
  printk(KERN_WARNING "bye from ioctl device\n");
  printk(KERN_WARNING "mydev unregistered\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
