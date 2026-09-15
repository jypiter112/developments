#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
  if(argc < 2){
    printf("Error\n");
    return errno;
  }

  FILE* fp;
  fp = fopen(argv[1], "r");

  char buf[4096];
  

  if(!fp){
    printf("Couldnt open path file %s\n", argv[1]);
    return errno;
  }

  while(fgets(buf, sizeof(buf), fp) != NULL){
    printf("%s", buf);
  }

  if(fp){
    fclose(fp);
  }
  return 0;
}
