#include "mydev_ioctl.h"
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydev"
#define CLASS_NAME "myclass"

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("IOCTL Device Test Module");
/* Grabbed form mydev_ioctl.h
#define MY_MAGIC 'Z'
#define WR_VALUE _IOW(MY_MAGIC, 1, char*)
#define RD_VALUE _IOR(MY_MAGIC, 2, char*)
*/
static char stored_value[256];

static char *mydev_devnode(const struct device *dev, umode_t *mode){
  if (mode) *mode = 0666;
  return NULL;
}

static long my_ioctl(struct file *fp, unsigned int cmd, unsigned long arg){
  char tmp[256];

  switch(cmd){
    case WR_VALUE:
      if(copy_from_user(tmp, (char __user *)arg, sizeof(tmp)))
        return -EFAULT;
      tmp[sizeof(tmp)-1] = '\0';
      strscpy(stored_value, tmp, sizeof(stored_value));
      printk(KERN_INFO "<char*><stored_value> changed to %s\n", stored_value);
      break;
    case RD_VALUE:
      if(copy_to_user((char __user *)arg, stored_value, strlen(stored_value) + 1))
        return -EFAULT;
      break;
    default:
      return -EINVAL;
  }
  return 0;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .unlocked_ioctl = my_ioctl,
};

static int __init my_init(void){
  // Allocate major dynamically (cat /proc/devices))
  if(alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0){
    pr_err("Failed to alloc major\n");
    return -1;
  }
  pr_info("Allocated major: %d\n", MAJOR(dev_num));

  // Register cdev (mknod)
  cdev_init(&my_cdev, &fops);
  my_cdev.owner = THIS_MODULE;
  if(cdev_add(&my_cdev, dev_num, 1) < 0){
    unregister_chrdev_region(dev_num, 0);
    return -1;
  }

  // Create /dev/mydev mkond + chmod
  my_class = class_create(CLASS_NAME);
  if(IS_ERR(my_class)){
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    return PTR_ERR(my_class);
  }
  my_class->devnode = mydev_devnode; // sets 666 permissions

  my_device = device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
  if(IS_ERR(my_device)){
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    return PTR_ERR(my_device);
  }

  pr_info("mydev: module loaded, /dev/%s created\n", DEVICE_NAME);
  return 0;
}

static void __exit my_exit(void){
  device_destroy(my_class, dev_num);
  class_destroy(my_class);
  cdev_del(&my_cdev);
  unregister_chrdev_region(dev_num, 1);
  pr_info("mydev: module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
