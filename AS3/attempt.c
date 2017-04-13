#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define GLOBAl_MODE 0
#define LOCAL_MODE 1
#define FIFO 2
#define LRU 3


int fl_Mode,gp_Mode;
int tlb_MaxSize; int tlbTreeSize = 0; int physpages;

int* tlbHits, *pageFaults, *pageOuts, *avs;

int free_frame_count =0;
int pageTable_count = 0;

int rotation =0;



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
  newEntry->process_id = rotation;
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
struct page_Table_Entry* deleteEntryFifo()
{
  /* base case */
  if(head == NULL)
    return NULL;



  /* If node to be deleted is head node */
  int freed_frame = head->FrameNumber;

  struct page_Table_Entry* temp = head;

  head = head->next;


  //free(head->prev);
  head->prev = NULL;

  //printf("got HERE\n");

  return temp;
}

// delete the end
struct page_Table_Entry* deleteEntryLru(){

  struct page_Table_Entry* temp = tail;

  int freed_frame = temp-> FrameNumber;
  int PageNumber = temp-> PageNumber;



  struct page_Table_Entry* prev = temp->prev;

  prev->next = NULL;
  //temp->prev = NULL;

  tail = prev;
  //free(temp);

  return temp;

}



int tlbHashFunc(int PN){
  return PN % tlb_MaxSize;
}

// allocate some memory for our hashtable
struct page_Table_Entry **createPageTable(int Size){
  struct page_Table_Entry **table = malloc( sizeof (struct page_Table_Entry) *Size);
  return table;
}

struct page_Table_Entry **resizePageTable(struct page_Table_Entry Table, int Size){

  struct page_Table_Entry **table = realloc(table, sizeof (struct page_Table_Entry) *Size);
  return table;


}

void hashInsert(struct page_Table_Entry **hashtable,  int PN, int FN){
  int arrayIndex = tlbHashFunc(PN);
  struct page_Table_Entry* newNode = createNewNode(PN, FN);
  hashtable[arrayIndex] = newNode;
}

struct page_Table_Entry *TLBlookup(struct page_Table_Entry **hashtable, int PN){
  int arrayIndex = tlbHashFunc(PN);
  if(hashtable[arrayIndex] != NULL) {
    if(hashtable[arrayIndex]->PageNumber == PN) {
      return hashtable[arrayIndex];
    }
  }
  return NULL;
}

// may need some work

struct page_Table_Entry *lookup(struct page_Table_Entry **hashtable, int PN){
  //int arrayIndex = HashFunc(PN);
  struct page_Table_Entry* newNode;


  struct page_Table_Entry* temp = head;

  if(head == NULL) {
		head = newNode;
		return newNode;
	}

	while(temp->next != NULL) {
    if(temp->PageNumber == PN){
      return temp;
    }
    temp = temp->next;
  }
  if(temp->PageNumber == PN) {
    return temp;
  }
  return NULL;
}


/////////////////////////
// To Allow Effictive Searching
// modified http://www.thegeekstuff.com/2013/02/c-binary-tree

struct bin_tree {
  struct page_Table_Entry** entry;
  struct bin_tree * right, * left;
  unsigned int PageNumber;
  struct page_Table_Entry* match; // pointer to page table entry
};

typedef struct bin_tree node;

struct bin_tree *root;     // root



void insert(node ** tree, unsigned int val, struct page_Table_Entry* match) {
    node *temp = NULL;

    if(!(*tree))
    {
        printf(" our value %x\n", val );
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

void deltree(node * tree) {
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

node* search(node ** tree, int val) {
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

void print_preorder(node * tree) {
    if (tree)
    {
        printf("%d\n",tree->PageNumber);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printf("%d\n",tree->PageNumber);
    }
}

int t =0;

node* del(node* root, int item) {

    node*savetemp=NULL;


    //print_postorder(root);
    //exit(1);
    if(root == NULL)
        printf("\nValue does not exist in tree!");



    if(item == root->PageNumber)
    {

        if(root->left==NULL&&root->right==NULL) //no child
        {

            root=NULL;


        }
        else if(root->left==NULL||root->right==NULL) //one child
        {

            if(root->left!=NULL) //left child
            {

                root=root->left;

            }
            else               //right child
            {

                root=root->right;

            }
        }
        else  if(root->left!=NULL&&root->right!=NULL) //both child
        {

            node* temp;
            savetemp=root->right->left;


            temp=root;

            root=root->right;

            printf(" checking %d\n",root->right->PageNumber);

            root->left=temp->left;

            printf("?\n");
        }
    }

    else
    {


            if(root->PageNumber < item)
            {




                root->right = del(root->right, item);

            }
            else
            {


                root->left=del(root->left, item);
            }
    }

    return root;
}



/* TLB structure */
typedef struct TLBNode {
    struct TLBNode *next;
    struct TLBNode *prev;
    int pageNumber;
    int pid;
} TLBNode;


typedef struct TLBQueue {
    int size;
    int maxSize;
    TLBNode *first;
    TLBNode *last;
} TLBQueue;


typedef struct TLBHashMap {
    int tlbSize;
    TLBNode **entries;
} TLBHashMap;


TLBNode* newTLBNode(int pageNumber, int pid) {
    TLBNode *temp = (TLBNode *)malloc(sizeof(TLBNode));
    temp->pageNumber = pageNumber;
    temp->prev = temp->next = NULL;



    temp->pid = pid;

    return temp;
}

TLBQueue *createTLBQueue(int size) {
    TLBQueue *queue = (TLBQueue *)malloc(sizeof(TLBQueue));
    queue->size = 0;
    queue->first = queue->last = NULL;
    queue->maxSize = size;
    return queue;
}

TLBHashMap *createTLBHash(int size) {
    TLBHashMap *hash = (TLBHashMap *) malloc(sizeof(TLBHashMap));
    hash->tlbSize = size;
    hash->entries = (TLBNode **) malloc(hash->tlbSize * sizeof(TLBNode*));
    return hash;
}

int TLBHasher(int PN, int tlbSize) {
  return PN % tlbSize;
}

int TLBFull(TLBQueue *queue){
    return queue->size == queue->maxSize;
}


int TLBEmpty(TLBQueue *queue) {
    return queue->last == NULL;
}


void delTLBPage(TLBQueue *queue) {
    if(TLBEmpty(queue)) {
        return;
    }
    if (queue->first == queue->last) {
        queue->first = NULL;
    }

    TLBNode* temp = queue->last;
    queue->last = queue->last->prev;

    if (queue->last)
        queue->last->next = NULL;

    free(temp);
    queue->size--;
}


void insertTLB(TLBQueue *queue, TLBHashMap *hash, int pageNumber, int pid) {
    if (TLBFull(queue)) {
        //printf("tlb full deleting\n");
        hash->entries[TLBHasher(queue->last->pageNumber, hash->tlbSize)] = NULL;
        delTLBPage(queue);
    }
    TLBNode *temp = newTLBNode(pageNumber, pid);
    temp->next = queue->first;

    if (TLBEmpty(queue)) {
        queue->last = queue->first = temp;
    }
    else  {
        queue->first->prev = temp;
        queue->first = temp;
    }

    hash->entries[TLBHasher(pageNumber, hash->tlbSize)] = temp;
    queue->size++;
}


void TLBSerach(TLBQueue *queue, TLBHashMap *hash, int pageNumber, struct page_Table_Entry** PageTable, int pid) {
    TLBNode *page = hash->entries[TLBHasher(pageNumber, hash->tlbSize)];
    if (page == NULL) {
        //printf("TLB Miss! New reference inserted, %d\n", pageNumber);
        pageTableLookUp(pageNumber, PageTable);
        insertTLB(queue, hash, pageNumber, pid);
    }
    // for collisions in table
    else if (page->pageNumber != pageNumber) {
        //printf("TLB Miss! New reference inserted, %d\n", pageNumber);
        pageTableLookUp(pageNumber, PageTable);
        insertTLB(queue, hash, pageNumber, pid);
    }
    else {


      tlbHits[rotation]++;  // TODO possibly change to account for v/i bits


      //printf("TLB hit! PN: %d\n", pageNumber);

      if(page != queue->first) {
        page->prev->next = page->next;
        if (page->next) {
          page->next->prev = page->prev;
        }
        if (page == queue->last) {
          queue->last = page->prev;
          queue->last->next = NULL;
        }
        page->next = queue->first;
        page->prev = NULL;

        page->next->prev = page;

        queue->first = page;
      }
    }
}


TLBQueue *TLBFlushQueue(TLBQueue *queue, int size) {
  free(queue);
  return createTLBQueue(tlb_MaxSize);
}

TLBHashMap *TLBFlushHash(TLBHashMap *hash, int size) {
  free(hash);
  return createTLBHash(tlb_MaxSize);
}
/* End of TLB structure */



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

        struct page_Table_Entry *tmp = deleteEntryLru();

        int process_id = tmp->process_id;

        //pf[process_id]++;
        pageOuts[process_id]++;



        //node* tmp = search(&root,tmp->PageNumber );
        del(root, PageNumber);

        freed_frame = tmp->FrameNumber;
        free(tmp);  // Don't run out of mem.

        // TODO update v/i bit of TLB
    }

      else if(fl_Mode == FIFO){



      struct page_Table_Entry *tmp = deleteEntryFifo();

      int process_id = tmp->process_id;

      //pf[process_id]++;
      pageOuts[process_id]++;

      //node* tmp = search(&root,tmp->PageNumber );
      freed_frame = tmp->FrameNumber;
      int delete_Page = tmp->PageNumber;


      free(tmp);

      //printf("Free Frame %d, %d\n",freed_frame, PageNumber);

      del(root, delete_Page);

      //printf("second Got here\n");

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

      if(free_frame_count == physpages){


          int index = PageOut(PageNumber);
          struct page_Table_Entry* tmp = InsertAtTail(PageNumber, index);
          printf("Got HERE\n");
          insert(&root, PageNumber, tmp);

      }
      // proceed
      else{
      printf("free_frame_count: %d\n",free_frame_count);

      //printf("Adding\n");

      //struct page_Table_Entry* newNode = createNewNode(PN, free_frame_count);
      struct page_Table_Entry* tmp = InsertAtTail(PageNumber, free_frame_count);
      insert(&root, PageNumber, tmp);

      free_frame_count++;
      //table[pageTable_count] =
      }
    }
    else{
      //printf("found %d inside PT\n", PageNumber);
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

  int PageNumber, value, x, TotalAvs;

  int *buffer;
  buffer = (int *)malloc(1*sizeof(int));



  int* address = (int*)malloc(1*sizeof(int));

  head = NULL;
  tail = NULL;    // doubly linked lists

  node *tmp;

  if (argc<=6){
    printf("Enter correct params:\n");
    return(0);

  }

  int pgsize = atof(argv[1]);
  tlb_MaxSize = atof(argv[2]);

  char* tlbentries_Mode = (char*) malloc(2);
  tlbentries_Mode = argv[3];

  if (*tlbentries_Mode == 'g'){gp_Mode = GLOBAl_MODE;}
  else if(*tlbentries_Mode == 'p'){gp_Mode = LOCAL_MODE;}

  else{
    printf("{g|p} wasn't entered\n");
    return 1;
  }

  int quantom_Pages = atof(argv[4]);
  physpages = atof(argv[5]);
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

  int traceFileAmount = argc - 7;

  //printf("%d\n",traceFileAmount);

  tlbHits = (int *)malloc(traceFileAmount*sizeof(int));
  pageFaults = (int *)malloc(traceFileAmount*sizeof(int));
  pageOuts = (int *)malloc(traceFileAmount*sizeof(int));
  avs = (int *)malloc(traceFileAmount*sizeof(int));

  struct file_struct *array = malloc(traceFileAmount * sizeof(struct file_struct));


  // get the file names and allocate the struct
  for(x = 0; x < traceFileAmount; x++){
    // create the structs
    struct file_struct *retVal = malloc(sizeof(struct file_struct));

    array[x] = *retVal;

    array[x].fp = fopen(argv[x+7], "r");
    array[x].finished = 0;
    printf("opened file %s\n",argv[x+7]);
    fread(buffer, 1, 3, array[x].fp);
  }



  struct page_Table_Entry** table = createPageTable(quantom_Pages);
  //struct page_Table_Entry** pageTable = createHashTable()

  //struct page_Table_Entry** tree = createHashTable(quantom_Pages);

  FILE* fp;
  TLBQueue* tlbQueue = createTLBQueue(tlb_MaxSize);
  TLBHashMap* tlbHash = createTLBHash(tlb_MaxSize);

  int i=0;

  int doneflag = 1;
  int addressx = 0;

  int shift_amount = pow(x,pgsize);

  while(doneflag){
  i =0;

  while(i++<quantom_Pages){

    if(array[rotation].finished == 0){
    fp = array[rotation].fp;
    }

    else{
      continue;
    }
    addressx = 0;

    for(x=0; x<1; x++){

      int check = fread(buffer, 1, 4, fp);


      if(check != 4){
        //printf("fileclosed %d\n", rotation);


        array[rotation].finished = 1; // if no more ytes read
        fclose(fp);
        break;
      }
      //printf("%x\n",* buffer);
      //getchar();

      addressx += buffer[0];
      memset(buffer, 0, sizeof(buffer));
    }

    //printf("%x\n",addressx );
    //getchar();

    unsigned int PageNumber = addressx / pgsize;

    //printf("our number %x\n", PageNumber );

    //PageNumber = PageNumber << shift_amount;
    //20xbits 10 0's

    TLBSerach(tlbQueue, tlbHash, PageNumber, table, 0);
    memset(address, 0, sizeof(address));


   }
  
  if(gp_Mode == LOCAL_MODE) {
    tlbQueue = TLBFlushQueue(tlbQueue, tlb_MaxSize);
    tlbHash = TLBFlushHash(tlbHash, tlb_MaxSize);
  }
   rotation++;
   //printf("rotation %d\n",rotation );

   rotation = rotation % (argc-7); // rotate through the files 0,1, .. 0,1

   if(quantom_Pages* i <= physpages)
   table = realloc(table, sizeof (struct page_Table_Entry) * quantom_Pages * i );  // resizePageTable every runthrough
    
   doneflag = 0;
   for(x = 0; x < traceFileAmount; x++){
     if(  array[x].finished == 0 ){
      doneflag = 1;
     }

     else{
         //printf("%d file closed\n",x);
     }

   }

 }

   //table = realloc(table, sizeof (struct page_Table_Entry) * physpages);   // change later

  //pageTableLookUp(1, table);
  //pageTableLookUp(2, table);
  //pageTableLookUp(3, table);
  //pageTableLookUp(3, table);


  //del(root, 2);





  // tests for hash table of size 2
  /*TLBSerach(tlbQueue, tlbHash, 10, table, 0);
  TLBSerach(tlbQueue, tlbHash, 11, table, 0);
  TLBSerach(tlbQueue, tlbHash, 1, table, 0);
  TLBSerach(tlbQueue, tlbHash, 135, table, 0);
  TLBSerach(tlbQueue, tlbHash, 654, table, 0);
  TLBSerach(tlbQueue, tlbHash, 43, table, 0);
  TLBSerach(tlbQueue, tlbHash, 655344, table, 0);
  TLBSerach(tlbQueue, tlbHash, 10, table, 0);
  TLBSerach(tlbQueue, tlbHash, 11, table, 0);
  TLBSerach(tlbQueue, tlbHash, 1, table, 0);
  TLBSerach(tlbQueue, tlbHash, 135, table, 0);
  TLBSerach(tlbQueue, tlbHash, 654, table, 0);
  TLBSerach(tlbQueue, tlbHash, 43, table, 0);
  TLBSerach(tlbQueue, tlbHash, 655344, table, 0);
  TLBSerach(tlbQueue, tlbHash, 655344, table, 0);
  TLBSerach(tlbQueue, tlbHash, 43, table, 0);
  TLBSerach(tlbQueue, tlbHash, 655344, table, 0);
  TLBSerach(tlbQueue, tlbHash, 43, table, 0);*/


  /* does a bunch of searches, all tlb should fail, then look into pt
   * which should then insert into pt */

  //int j;
  //for(j = 0; j < 10000; j++) {
  //  TLBSerach(tlbQueue, tlbHash, j, table, 0);
  //}

  /*tlbQueue = TLBFlushQueue(tlbQueue, tlb_MaxSize);
  tlbHash = TLBFlushHash(tlbHash, tlb_MaxSize);
  TLBSerach(tlbQueue, tlbHash, 10, table, 0);
  TLBSerach(tlbQueue, tlbHash, 11, table, 0);
  TLBSerach(tlbQueue, tlbHash, 43, table, 0);*/
  //printf("root: %d, lchild: %d, rchild: %d\n", tlbRoot->PageNumber, tlbRoot->left->PageNumber, tlbRoot->right->PageNumber);






  while(head->next != NULL){
   struct page_Table_Entry* temp = head;
   int q = temp->process_id;
   avs[q]++;
   head = head->next;
  }

   struct page_Table_Entry* temp2 = head;
   int q = temp2->process_id;
   avs[q]++;

  for(x=0; x< traceFileAmount; x++){

    printf("Tlbhits%d: %d pf%d: %d \npageouts%d: %d avs%d: ",x, tlbHits[x], x, pageFaults[x], x, pageOuts[x], x);
    printf("%d\n", avs[x]);


  }




   //insert(&root, 1);
   //insert(&root, 2);
   //insert(&root, 3);

   /*printf("Pre Order Display Bit Tree\n");
   print_preorder(root);

   printf("Printing our PageTable\n");
   Print();*/

   // Free all memory




   //free(pageTable_Mode);
   //free(tlbentries_Mode);

   exit(1);
}
