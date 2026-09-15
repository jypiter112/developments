#include <stdio.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <sys/ioctl.h>  // ioctl()
#include <errno.h>      // errno (optional but useful)
#include <string.h>     // strerror (optional)
                        //
#define MY_MAGIC 'M'
#define WR_VALUE _IOW(MY_MAGIC, 1, int)
#define RD_VALUE _IOR(MY_MAGIC, 2, int)

int main(){
  int fd = open("/dev/mydev", O_RDWR | O_CLOEXEC);
  int val = 42;
  int ret;

  if(fd < 0) {
    perror("open");
    return 1;
  }
  
  // send value to kernel (42)
  if(ioctl(fd, WR_VALUE, &val) < 0){
    perror("ioctl WR_VALUE");
    return 1;
  }

  // read value
  if(ioctl(fd, RD_VALUE, &val) < 0){
    perror("ioctl RD_VALUE");
    return 1;
  }
  printf("Read value %d\n", val);
  close(fd);

  return 0;
}
