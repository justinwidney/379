#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    FILE *fp;

    fp = fopen("heapsort-trace.bin", "r");
    int *buffer;

    int* Page_Table;

    buffer = (int *)malloc(1*sizeof(int));

    int* address = (int*)malloc(1*sizeof(int));

    buffer = (int *)malloc(10*sizeof(int));

    int i=0;
    int x=0;
    while(i++<20){

      for(x=0; x<4; x++){
      fread(buffer, 1, 1, fp);
      address[0]+=buffer[0];

      memset(buffer, 0, sizeof(buffer));
      printf("temp = %x\n", address[0]);
      }


      int shift = address[0] >> 12;
      int offset = address[0] << 20;
      //printf("? %d ?", offset);
      offset = offset >> 20;

      //printf("%x\n", shift);
      printf("final %04x\n", address[0]);
      printf("PN %d && offset %d\n",shift, offset);
      memset(address, 0, sizeof(address));



    }
    fclose(fp);
    return(0);


}
