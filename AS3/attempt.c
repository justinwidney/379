#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define GLOBAl_MODE 0
#define LOCAL_MODE 1
#define FIFO 2
#define LRU 3


int fl_Mode,gp_Mode;

int free_frame_count =0;
int pageTable_count = 0;

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
struct page_Table_Entry* InsertAtTail(int PT, int FrameNumber) {

  //int hash_val = HashFunc(PN);
  //page_Table_Entry *entry = htable[hash_val];




	struct page_Table_Entry* temp = head;
	struct page_Table_Entry* newNode = createNewNode(PT, FrameNumber);

	if(head == NULL) {
		head = newNode;
		return newNode;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
  tail = newNode;

  return newNode;

}

// delete the first in
int deleteEntryFifo()
{
  /* base case */
  if(head == NULL)
    return -1;

  /* If node to be deleted is head node */
  int freed_frame = head->FrameNumber;

  head = head->next;
  free(head->prev);
  head->prev = NULL;


  return freed_frame;
}

// delete the end
int deleteEntryLru(){

  struct page_Table_Entry* temp = tail;

  int freed_frame = temp-> FrameNumber;
  struct page_Table_Entry* prev = temp->prev;

  prev->next = NULL;
  //temp->prev = NULL;

  tail = prev;
  free(temp);

  return freed_frame;

}



int HashFunc(int PN){
  return PN % 25;
}

// allocate some memory for our hashtable
  struct page_Table_Entry **createHashTable(int hashSize){
  struct page_Table_Entry **table = malloc( sizeof (struct page_Table_Entry) *hashSize);
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

// may need some work

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


/////////////////////////
// To Allow Effictive Searching
// modified http://www.thegeekstuff.com/2013/02/c-binary-tree

struct bin_tree {
  struct page_Table_Entry** entry;
  struct bin_tree * right, * left;
  int PageNumber;
  struct page_Table_Entry* match; // pointer to page table entry

};

typedef struct bin_tree node;

struct bin_tree *root;     // root


void insert(node ** tree, int val, struct page_Table_Entry* match)
{
    node *temp = NULL;
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->PageNumber= val;
        temp->match = match;
        *tree = temp;
        return;
    }

    if(val < (*tree)->PageNumber)
    {
        insert(&(*tree)->left, val, match);
    }
    else if(val > (*tree)->PageNumber)
    {
        insert(&(*tree)->right, val, match);
    }

}

void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

node* search(node ** tree, int val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val < (*tree)->PageNumber)
    {
        search(&((*tree)->left), val);
    }
    else if(val > (*tree)->PageNumber)
    {
        search(&((*tree)->right), val);
    }
    else if(val == (*tree)->PageNumber)
    {
        return *tree; // Return pointer to a Entry
    }
}

void print_preorder(node * tree)
{
    if (tree)
    {
        printf("%d\n",tree->PageNumber);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}


//////////////////////////






// look up said entry, add it
// Return -1 for PageFault, or X for position in struct
int pageTableScan(int PageNumber){

  node* tmp = search(&root, PageNumber);

  if (tmp)
    {
    struct page_Table_Entry * found = tmp->match;
    return found->FrameNumber;
    }

  else{ return -1; }


}

// proceed using mode
int PageOut(int PageNumber){

    int freed_frame = 0;

    if( fl_Mode == LRU){

        freed_frame = deleteEntryLru();
        // TODO update v/i bit of TLB
    }

      else if(fl_Mode == FIFO){

       freed_frame = deleteEntryFifo();
       // TODO update v/i bit of TLB
    }

    else{
      printf("What did we do?\n");
      exit(1);
    }

    return freed_frame;

}

int pageTableLookUp(int PageNumber, struct page_Table_Entry** PageTable){


    int value = pageTableScan(PageNumber);



    // Isn't in PageTable
    if (value == -1){

      // Page Out
      if(free_frame_count == 100){

          int index = PageOut(PageNumber);

          struct page_Table_Entry* tmp = InsertAtTail(PageNumber, index);
          insert(&root, PageNumber, tmp);

      }

      // proceed
      else{
      //struct page_Table_Entry* newNode = createNewNode(PN, free_frame_count);

      struct page_Table_Entry* tmp = InsertAtTail(PageNumber, free_frame_count);
      insert(&root, PageNumber, tmp);

      free_frame_count++;
      //table[pageTable_count] =
      }

    }

    else{
      printf("found %d inside PT\n", PageNumber);
      //TODO update TLB, can be done inside TLB function

    }




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

  int *buffer;
  buffer = (int *)malloc(10*sizeof(int));

  int* address = (int*)malloc(1*sizeof(int));

  head = NULL;
  tail = NULL;    // doubly linked lists


  node *tmp;



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

   //struct page_Table_Entry** tree = createHashTable(quantom_Pages);

   FILE* fp = fopen(argv[7], "r");


   int i=0;

   while(i++<20){

     for(x=0; x<4; x++){
     fread(buffer, 1, 1, fp);
     address[0]+=buffer[0];

     memset(buffer, 0, sizeof(buffer));
     }


     int shift = address[0] >> 12;
     int offset = address[0] << 20;

     int PageNumber = shift >> 12; //20xbits 10 0's

     //printf("? %d ?", offset);
     offset = offset >> 20;  // ignore



     //printf("%x\n", shift);
     //printf("final %04x\n", address[0]);
     printf("PN %d && offset %d\n",PageNumber, offset); // Page Number will be 0 for a while as shift pageSize bits over
     memset(address, 0, sizeof(address));
   }


   pageTableLookUp(1, table);
   pageTableLookUp(2, table);
   pageTableLookUp(3, table);
   pageTableLookUp(3, table);


   //insert(&root, 1);
   //insert(&root, 2);
   //insert(&root, 3);

   printf("Pre Order Display Bit Tree\n");
   print_preorder(root);

   printf("Printing our PageTable\n");
   Print();

   // Free all memory




   //free(pageTable_Mode);
   //free(tlbentries_Mode);

   exit(1);
}
