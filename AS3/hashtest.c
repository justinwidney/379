#include <stdio.h>
#include<stdlib.h>


struct page_Table_Entry{
  int PageNumber;
  int FrameNumber;
  int valid_bit;
  int process_id;
  struct page_Table_Entry* next;
  struct page_Table_Entry* prev;
  int key; // for hashing
};


struct page_Table_Entry* head;  // our first entry


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

  int PageNumber, value;
  head = NULL;

  //InsertAtTail(2,3);
  //InsertAtTail(4,5);
  //InsertAtTail(5,6);
  //InsertAtHead(1,7);

  struct page_Table_Entry** table = createHashTable(25);

  hashInsert(table, 1,2);
  hashInsert(table, 2,3);
  hashInsert(table, 3,4);
  Print();

  struct page_Table_Entry* temp = lookup(table, 2);

  printf("found: %d\n", temp->PageNumber );





}
