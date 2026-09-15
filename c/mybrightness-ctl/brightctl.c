#include <stdlib.h>
#include <stdio.h>

int write_kbd(char* val, char* kbd_path)
{
  int sv = atoi(val);
   if(sv < 0){
     sv = 0;
   }
   if(sv > 2){
     sv = 2;
   }
   
   char c[16];
   size_t s = sprintf(c, "%d", sv);
   if (s <= 0){
     return -1;
   }
   FILE *f;
   if((f = fopen(kbd_path, "w"))) {
     if(fwrite(c, 1, s, f) < s){
       return -1;
     }
   }
   else {
     fprintf(stdout, "Fail\n");
     return -1;
   }
   fclose(f);
   fprintf(stdout, "Wrote value %s\n", val);
   return 0;
}

int main(int argc, char *argv[])
{
  char* kbd_path = "/sys/class/leds/dell::kbd_backlight/brightness";
  write_kbd(argv[1], kbd_path);
  return EXIT_SUCCESS;
}
