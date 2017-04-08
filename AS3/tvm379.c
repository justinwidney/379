#include <stdio.h>

#define GLOBAl_MODE 0
#define LOCAL_MODE 1
#define FIFO 2
#define LRU 3



/*
1. Read BYTE && GET PN
2. Check if it is TLB
2a) HIT, move on
2b) Miss, check if it is in PT
3a) Hit, Update TLB_top
3b) Miss, Update TLB and page in
4a) free memory frame, swap it in
4b) replace frame using LRU
5) loop quantom times per file
*/








int* stack;
int gp_Mode, fl_Mode;

// Arrays for stats at the end.
int* tlbHits;
int* pfX;
int* avs;

int TLB_top = -1;   // for stacks
int PT_Top = -1;
int free_frames_count = 0;
int PT_Front = 0;  // for queue

int* free_frames_list;

std::vector<int> Page_Table(10);  // 32bits - 2^Xpagesize = Xbits   2^xbits size


// for use of quantom pages
typedef struct file_struct {
  FILE fp*;
  int finished;
  int process_id;

};

// https://gist.github.com/mycodeschool/7429492 for double linked list
// store for a page table entry
// *Note can be used for TLB as well

typdef struct page_Table_Entry{
  int PageNumber;
  int FrameNumber;
  int valid_bit;
  int process_id;
  struct page_Table_Entry* next;
  struct page_Table_Entry* prev;
  int key; // for hashing
}

typedef struct TLB_



struct page_Table_Entry* head;  // our first entry

struct page_Table_Entry* createNewNode(int PT, int FrameNumber){
  struct page_Table_Entry* newEntry =
  (struct page_Table_Entry*)malloc(sizeof(struct page_Table_Entry));
  newEntry->PageNumber = PT;
  newEntry->FrameNumber = FrameNumber;
  newEntry->next = NULL;
  newEntry->prev = NULL;
  return newEntry

}

//Inserts a Node at head of doubly linked list
void InsertAtHead(int PT, int FrameNumber) {
	struct Node* newNode = GetNewNode(PT, FrameNumber);
	if(head == NULL) {
		head = newNode;
		return;
	}
	head->prev = newNode;
	newNode->next = head;
	head = newNode;
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(int PT, int FrameNumber) {
	struct Node* temp = head;
	struct Node* newNode = GetNewNode(PT, FrameNumber);
	if(head == NULL) {
		head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
}



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
// http://stackoverflow.com/questions/24353707/hash-table-with-linked-list

page_Table_Entry **createHashTable(int hashSize)
{
    ValueObjectType **table = malloc( sizeof(page_Table_Entry *) * hashSize);
    return table;
}

void hashInsert(page_Table_Entry **hashTable, int key, page_Table_Entry *value)
{
    int arrayIndex = hashingFunction(key);
    if ( hashTable[arrayIndex] != NULL ) traverseLinkedListAndAdd(...);
    else hashTable[arrayIndex] = value;
}

page_Table_Entry *lookupByKey(page_Table_Entry **hashTable, int key)
{
    int arrayIndex = hashingFunction(key);
    return traverseLinkedListAndReturnObjectWithKey(...);
}


int searchMatch(int PN, int PID){

  int hashIndex = hashCode(PN, PID);


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

    for(x=0; x<4; x++){
    fread(buffer, 1, 1, fp);
    address[0]+=buffer[0];

    memset(buffer, 0, sizeof(buffer));
    printf("temp = %x\n", address[0]);
    }


    int shift = address[0] >> 12;
    int PageNumber = shift >> 12; //20xbits 10 0's

    //int offset = address[0] << 20;
    //offset = offset >> 20;  // ignore


      if(gp_Mode== GLOBAl_MODE){
        process_id = 0;
      }
      else{

      for(x=0; x<argc-6; x++){
        continue;
        // search struct for fp* and get process_id
      }

      }


      Hit = searchTLB(PageNumber, process_id);  // see if it is in the TLB

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
