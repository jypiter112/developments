#include "linux/device/class.h"
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/err.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Device driver test.");

#define DEV_NAME "mydev"
#define CLASS_NAME "myclass"
#define mem_size 1024
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
uint8_t *kernel_buff;

static int __init mod_init(void);
static void __exit mod_exit(void);
static int etx_open(struct inode*, struct file*);
static int etx_release(struct inode*, struct file*);
static ssize_t etx_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t etx_write(struct file*, const char __user*, size_t, loff_t*);

// hooks
static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = etx_read,
  .write = etx_write,
  .open = etx_open,
  .release = etx_release,
};
// Called on every open(fd) of our device filej
static int etx_open(struct inode *inode, struct file *file){
  pr_info("Device file opened\n");
  return 0;
}
static int etx_release(struct inode *inode, struct file *file){
  pr_info("Device file closed\n");
  return 0;
}
static ssize_t etx_read(struct file *fp, char __user *buf, size_t len, loff_t *off){
  if(copy_to_user(buf, kernel_buff, mem_size)){
    pr_err("Data read: Err!\n");
  }
  pr_info("Data read: Done!\n");
  return mem_size;
}
static ssize_t etx_write(struct file *fp, const char __user *buf, size_t len, loff_t *off){
  if(copy_from_user(kernel_buff, buf, len)){
    pr_err("Data write :Err!\n");
  }
  pr_info("Data write : done!\n");
  return len;
}
static int __init mod_init(void){
  if((alloc_chrdev_region(&dev, 0, 1, DEV_NAME)) < 0){
    pr_err("Cant alloc major\n");
    return -1;
  }
  pr_info("Mahor = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

  // create cdev struct
  cdev_init(&etx_cdev, &fops);
  if((cdev_add(&etx_cdev, dev, 1)) < 0){
    pr_err("Cant add dev to system\n");
    goto r_class;
  }
  if(IS_ERR(dev_class = class_create(CLASS_NAME))){
    pr_err("Cant create struct class\n");
    goto r_class;
  }
  if(IS_ERR(device_create(dev_class, NULL, dev, NULL, DEV_NAME))){
    pr_info("Cant create device 1\n");
    goto r_device;
  }
  // Allocating physical memory
  if((kernel_buff = kmalloc(mem_size, GFP_KERNEL)) == 0){
    pr_err("Cant alloc mem in kernel\n");
    goto r_device;
  }
  strcpy(kernel_buff, "Hello world");
  pr_info("Device driver inserted\n");
  return 0;

  // Exceptions
r_class:
  class_destroy(dev_class);
r_device:
  unregister_chrdev_region(dev, 1);
  return 0;
}
static void __exit mod_exit(void){
  kfree(kernel_buff);
  device_destroy(dev_class, dev);
  class_destroy(dev_class);
  unregister_chrdev_region(dev, 1);
  pr_info("Device removed\n");
}
module_init(mod_init);
module_exit(mod_exit)
