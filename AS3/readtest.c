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
      //int shift = buffer[0] >> 3;

      //printf("%x\n", shift);
      printf("final %04x\n", address[0]);
      printf("value %d\n",address[0]);
      memset(address, 0, sizeof(address));



    }
    fclose(fp);
    return(0);


}
