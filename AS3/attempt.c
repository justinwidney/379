#include <stdio.h>
#include <stdlib.h>


#define GLOBAl_MODE 0
#define LOCAL_MODE 1
#define FIFO 2
#define LRU 3


int fl_Mode,gp_Mode;

struct page_Table_Entry{
  int PageNumber;
  int FrameNumber;
  int valid_bit;
  int process_id;
  struct page_Table_Entry* next;
  struct page_Table_Entry* prev;
  int key; // for hashing
};


// For using Files:
//////////////////
struct file_struct {
  FILE* fp;
  int finished;
  int process_id;

};

struct file_struct* newFileStruct (char* text){

  struct file_struct *retVal = malloc(sizeof(struct file_struct));
  if(retVal == NULL){
    return NULL;
  }

  retVal->fp = fopen(text, "r");
  retVal->finished = 0;

  return retVal;
}

void delFileStruct (struct file_struct* structure){
  if( structure != NULL){
    fclose(structure->fp);
    free(structure);
  }
}


////////////


struct page_Table_Entry* head;  // our first entry
struct page_Table_Entry* tail;



struct page_Table_Entry* createNewNode(int PT, int FrameNumber){
  struct page_Table_Entry* newEntry =
  (struct page_Table_Entry*)malloc(sizeof(struct page_Table_Entry));
  newEntry->PageNumber = PT;
  newEntry->FrameNumber = FrameNumber;
  newEntry->next = NULL;
  newEntry->prev = NULL;
  return newEntry;

}

//Inserts a Node at head of doubly linked list
void InsertAtHead(int PN, int FrameNumber) {

  //int hash_val = HashFunc(PN);
  //page_Table_Entry *entry = htable[hash_val];



	struct page_Table_Entry* newNode = createNewNode(PN, FrameNumber);
	if(head == NULL) {
		head = newNode;
    tail = newNode;
		return;
	}

	head->prev = newNode;
	newNode->next = head;
	head = newNode;



}


//Inserts a Node at tail of Doubly linked list
void InsertAtTail(int PT, int FrameNumber) {

  //int hash_val = HashFunc(PN);
  //page_Table_Entry *entry = htable[hash_val];




	struct page_Table_Entry* temp = head;
	struct page_Table_Entry* newNode = createNewNode(PT, FrameNumber);

	if(head == NULL) {
		head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
  tail = newNode;


}

// delete the first in
void deleteEntryFifo()
{
  /* base case */
  if(head == NULL)
    return;

  /* If node to be deleted is head node */
  head = head->next;
  free(head->prev);
  head->prev = NULL;


  return;
}

// delete the end
void deleteEntryLru(){

  struct page_Table_Entry* temp = tail;
  struct page_Table_Entry* prev = temp->prev;

  prev->next = NULL;
  //temp->prev = NULL;

  tail = prev;
  free(temp);

}



int HashFunc(int PN){
  return PN % 25;
}

// allocate some memory for our hashtable
  struct page_Table_Entry **createHashTable(int hashSize){
  struct page_Table_Entry **table = malloc( sizeof(struct page_Table_Entry)*hashSize);
  return table;
}

void hashInsert(struct page_Table_Entry **hashtable,  int PN, int FN){
int arrayIndex = HashFunc(PN);

  if(hashtable[arrayIndex]!= NULL){
  InsertAtTail(PN,FN);
  }
  else{
    printf("creating new node\n");
    struct page_Table_Entry* newNode = createNewNode(PN, FN);
    InsertAtTail(PN,FN);
    hashtable[arrayIndex] = newNode;
  }

}

struct page_Table_Entry *lookup(struct page_Table_Entry **hashtable, int PN){
  int arrayIndex = HashFunc(PN);

  struct page_Table_Entry* newNode;


  struct page_Table_Entry* temp = head;

  if(head == NULL) {
		head = newNode;
		return newNode;
	}

	while(temp->next != NULL) {

  if(temp->PageNumber == PN){
    printf("found it\n");
    return temp;
  }

  temp = temp->next;
  }

  return NULL;

}




//void remove(int PN){
    //int hash_val = HashFunc(PN);
    //page_Table_Entry *entry = htable[hash_val];

//}

void Print() {
	struct page_Table_Entry* temp = head;
	printf("Forward: \n");
	while(temp != NULL) {
		printf("%d ",temp->PageNumber);
    printf("%d\n",temp->FrameNumber);
		temp = temp->next;
	}
	printf("\n");
}

int main(int argc, char *argv[]) {

  int PageNumber, value, x;
  head = NULL;
  tail = NULL;


  if (argc<=6){
    printf("Enter correct params:\n");
    return(0);

  }

  int pgsize = atof(argv[1]);
  int tlbentries = atof(argv[2]);


  char* tlbentries_Mode = (char*) malloc(2);
  tlbentries_Mode = argv[3];

  if (*tlbentries_Mode == 'g'){gp_Mode = GLOBAl_MODE;}
  else if(*tlbentries_Mode == 'p'){gp_Mode = LOCAL_MODE;}

  else{
    printf("{g|p} wasn't entered\n");
    return 1;
    }



  int quantom_Pages = atof(argv[4]);
  int phys_Pages = atof(argv[5]);
  char* pageTable_Mode = (char*) malloc(2);
  pageTable_Mode = argv[6];


  if (*pageTable_Mode == 'f'){
    fl_Mode = FIFO;
  }
  else if(*pageTable_Mode == 'l'){
    fl_Mode = LRU;
  }

  else{
    printf("{f|l} wasn't entered\n");
    return 1;
  }

   int traceFileAmount = argc - 6;

   //printf("%d\n",traceFileAmount);


   struct file_struct *array = malloc(traceFileAmount * sizeof(struct file_struct));


   // get the file names and allocate the struct
   for(x = 0; x < traceFileAmount; x++){
     // create the structs
     struct file_struct *retVal = malloc(sizeof(struct file_struct));

     array[x] = *retVal;

     //array[x].fp = fopen(argv[x+6], "r");
     array[x].finished = 0;
   }

   struct page_Table_Entry** table = createHashTable(quantom_Pages);

   // Free all memory


   //free(pageTable_Mode);
   //free(tlbentries_Mode);

   exit(1);
}
