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


/* page_Table_Entry **pageTable, **top;

void createHashTable(){

  int i;

  pageTable = page_Table_Entry*[25];
  top = page_Table_Entry*[25];

  for(i=0; i< 25; i++){
    pageTable[i] = NULL;
    top[i] = NULL;
  }



}

// may need improvement
int HashFunc(int PN){
  return PN % 25;
}
*/

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

  InsertAtTail(2,3);
  InsertAtTail(4,5);
  InsertAtTail(5,6);
  InsertAtHead(1,7);
  Print();






}
