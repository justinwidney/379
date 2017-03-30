#include <stdio.h>

#define GLOBAl_MODE 0
#define LOCAL_MODE 1
#define FIFO 2
#define LRU 3




// read in a pgsize, tlbentries { g| p} quantom physpages { f|l } tracefiles
int main(int argc, char *argv[]) {


  int pgsize = atof(argv[1]);
  int tlbentries = atof(argv[2]);
  char* tlbentries_Mode = (char*) malloc(1);
  tlbentries_Mode = argv[3];

  int gp_Mode = 0;

  if (tlbentries_Mode == 'g'){
    gp_Mode = GLOBAl_MODE;
  }
  else if(tlbentries_Mode == 'l'){
    gp_Mode = LOCAL_MODE;
  }

  else{
    printf("{g|p} wasn't entered\n");
    return 1;
    }

  int quantom_Pages = atof(argv[4]);
  char* quantom_Mode = (char*)malloc(1);
  quantom_Mode = argv[5];

  int fl_Mode;

  if (tlbentries_Mode == 'f'){
    fl_Mode = FIFO;
  }
  else if(tlbentries_Mode == 'l'){
    fl_Mode = LRU;
  }

  else{
    printf("{f|l} wasn't entered\n");
    return 1;
    }

  int traceFileAmount = argc - 6;
  int x;

  char **traceFiles;
  traceFiles = malloc(traceFileAmount * sizeof(char*));

  for(x =0; x < traceFileAmount; x++){
    traceFiles[x] = malloc(strlen(argv[x+6]) * sizeof(char));
    strcpy(traceFiles[x],argv[x+6]);

  }









  // free memory at end
  for(x=0; x< traceFileAmount; x++){
    free(traceFiles[x]);

  }
  free(traceFiles);
  free(quantom_Mode);
  free(tlbentries_Mode);





}
