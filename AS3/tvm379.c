#include <stdio.h>

#define GLOBAl_MODE 0
#define LOCAL_MODE 1
#define FIFO 2
#define LRU 3

int* stack;
int gp_Mode, fl_Mode;

// Arrays for stats at the end.
int* tlbHits;
int* pfX;
int* avs;

int TLB_top = -1;   // for stacks
int PT_Top = -1;

int PT_Front = 0;  // for queue

int* free_frames_list;

std::vector<int> Page_Table(10);  // 32bits - 2^Xpagesize = Xbits   2^xbits size


// for use of quantom pages
typedef struct file_struct {
  FILE fp*;
  int finish;
};

// create our structure for the file
struct file_struct *newStruct (char* text){

  struct file_struct *retVal = malloc(sizeof(struct file_struct));
  if(retVal == NULL){
    return NULL;
  }
  retVal->fp = fopen(text, "r");
  retVal->finish = 0;

  return retVal;


}

/*
* Update the PT
* if free mem, append to PT
* Else use LRU OR FIFO
*/
int updatePT(int PN){
  int HIT;

  // STACK
  if(fl_Mode == LRU){

  }

  // QUEUE
  else if(fl_Mode == FIFO){

  }

  return HIT;
}

// locate the entry int the Page Table
int findPTMatch(int PN){

}

// locate the PN in the TLB
int findTLBMatch(int PN, int PID){



}

/*
* Update using LRU
*/

int updateTLB(int PN){
    int HIT;

      //search




      //FOUND


      //PAGE # () | Frame # () | ASID PID | V/I
      if(fl_Mode == GLOBAl_MODE){

        // Not Found
        top++;
        stack[top] = PN;

      }

      //PAGE # () | Frame # ()| V/I
      else if(fl_Mode == LOCAL_MODE) {

        // Not Found
        top++;
        stack[top] = PN;


      }

    return HIT;
}



// clear the struct
void delVector (struct file_struct *struct){
  if( struct != NULL){
    fclose(retVal->fp);
    free(struct);
  }
}


// read in a pgsize, tlbentries { g| p} quantom physpages { f|l } tracefiles
int main(int argc, char *argv[]) {


  int pgsize = atof(argv[1]);


  int tlbentries = atof(argv[2]);
  stack = (int *)malloc(tlbentries * sizeof(int)); // allocate our TLB


  char* tlbentries_Mode = (char*) malloc(1);
  tlbentries_Mode = argv[3];



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

  tlbHits = (int*) malloc(traceFileAmount * sizeof(int)); // Hits for stats per process
  pfX = (int*) malloc(traceFileAmount * sizeof(int)); // Page faults per process
  avsX = (int*) malloc(traceFileAmount * sizeof(int)); // average of Page

  char **traceFiles;
  traceFiles = malloc(traceFileAmount * sizeof(char*));

  file_struct *array = malloc(traceFileAmount * sizeof(struct));


  // get the file names and allocate the struct
  for(x =0; x < traceFileAmount; x++){
    traceFiles[x] = malloc(strlen(argv[x+6]) * sizeof(char));
    strcpy(traceFiles[x],argv[x+6]);

    // create the structs
    file_struct *struct = newStruct(argv[x+6]);
    array[x] = file_struct;


  }


  // read
  int finish = 0 ;
  int HIT =0, PT_HIT=0;
  // loop until end of last tracefile
  x = 0;

  while(1){

    for(i =0; i < quantom_Pages; i++){

      int PN = read_file(array[x]->fp); // read the file

      Hit = TLB(PN);

      if(HIT){
        continue;
      }
      else{
        PT_HIT = PT(PN);
      }

      // start of PT
      if(PT_HIT){
        updateTLB();
      }

      else{
        updatePT();
        updateTLB();

      }

    }

    // flush the TLB
    if(gp_Mode == LOCAL_MODE){

    }

    x++;
    if(array[x]->finish == 1){
      x++;
    }

    x = x%traceFileAmount; // loop back around


  }








  // free memory at end
  for(x=0; x< traceFileAmount; x++){
    free(traceFiles[x]);

  }
  free(traceFiles);
  free(quantom_Mode);
  free(tlbentries_Mode);





}
