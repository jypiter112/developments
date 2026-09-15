#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "myconf"
#define DEV_CLASS = "myconf_class"
#define MAX_WRITE_SIZE 4096

#define DEV_MAGIC "Z"
#define WR_VALUE _IOW(DEV_MAGIC, 0, char *)
#define RD_VALUE _IOR(DEV_MAGIC, 1, char *)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Test");
MODULE_DESCRIPTION("Char device config");

static char buf[246];

static char *myconf_devnode(const struct device *dev, umode_t *mode) {
  if (mode) {
    *mode = 0666;
  }
  return NULL;
}

static long myconf_ioctl(struct fp *fp, unsigned int cmd, unsigned long arg) {
  char _kbuf[256];
  switch (cmd) {
  case WR_VALUE:
    if (copy_from_user(_kbuf, (char __user *)arg, sizeof(_kbuf)))
      return -EFAULT;
    _kbuf[sizeof(_kbuf) - 1] = '\0';
    strscpy(buf, _kbuf, sizeof(buf));
    pr_info("myconf: received value: %s\n", buf);
    break;
  case RD_VALUE:
    if (copy_to_user((char __user *)arg, buf, sizeof(buf)))
      return -EFAULT;
    break;
  default:
    return -EINVAL;
  }
  return 0;
}

/*
 * Mod init and exit functions
 */
static int __init ipconf_init(void) {
  pr_info("myconf loaded");
  return 0;
}

static void __exit ipconf_exit(void) { pr_info("myconf unloaded"); }

module_init(ipconf_init);
module_exit(ipconf_exit);
