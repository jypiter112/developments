#ifndef MYDEV_IOCTL_H
#define MYDEV_IOCTL_H

#include <linux/ioctl.h>

#define MY_MAGIC 'Z'
#define WR_VALUE _IOW(MY_MAGIC, 1, char*)
#define RD_VALUE _IOR(MY_MAGIC, 2, char*)

#endif
