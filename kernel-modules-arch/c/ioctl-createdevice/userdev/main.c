#include "../mydev_ioctl.h"
#include <sys/ioctl.h>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
/*
#define MY_MAGIC 'Z'
#define WR_VALUE _IOW(MY_MAGIC, 1, char*)
#define RD_VALUE _IOR(MY_MAGIC, 2, char*)
*/
int main(int argc, char *argv[])
{ 
  if(argc < 2){
    fprintf(stderr, "Usage: %s <value>\n", argv[0]);
    return 1;
  }

  int fd = open("/dev/mydev", O_RDWR|O_CLOEXEC); 
  if(fd < 0) {
    perror("open");
    return 1;
  }
  
  if(ioctl(fd, WR_VALUE, argv[1]) < 0){
    perror("WR_VALUE");
    close(fd);
    return 1;
  }
  
  char read_val[256];
  if(ioctl(fd, RD_VALUE, read_val) < 0){
    perror("RD_VALUE");
    close(fd);
    return 1;
  }

  printf("Read value %s\n", read_val);
  close(fd);
  return 0;
}
