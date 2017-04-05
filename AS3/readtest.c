#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    FILE *fp;

    fp = fopen("heapsort-trace.bin", "r");
    int *buffer;

    buffer = (int *)malloc(1*sizeof(int));

    int i=0;
    while(i++<1000000){
      fread(buffer, 4, 1, fp);
      printf("%x\n", buffer[0]);
      memset(buffer, 0, sizeof(buffer));

    }
    fclose(fp);
    return(0);


}
