#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int free_frames_list[100]; // size of physpages
int free_frames_count;
int Page_Table[100];      // 2^32 - 2^x of pagesize
int Page_Table_count;


int UpdatePT(int PN);
int searchPT(int PN);
int getFrame();


int front;
int rear;
int itemCount;


// queue stuff
void insert(int data) {

   if(!isFull()) {

      if(rear == MAX-1) {
         rear = -1;
      }

      Page_Table[++rear] = data;
      itemCount++;
   }
}

int removeData() {
   int data = Page_Table[front++];

   if(front == MAX) {
      front = 0;
   }

   itemCount--;
   return data;
}

// stack stuff
int pop() {
   int data;

   if(!isempty()) {
      data = stack[top];
      top = top - 1;
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}

int push(int data) {

   if(!isfull()) {
      top = top + 1;
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}







int getFrame(){
    return free_frames_count;
}

int UpdatePT(int PN){


    // load stuff into page table until it is full
    if(Page_Table_count == 100){
      printf("Do Swapping Code");
      exit(1);
    }

    if(free_frames_count == 100){
      printf("LRU OR FIFO");

      //LRU stack, remove array[0]


      //FIFO QUEUE, remove bottom of stack array[sizeof(array)-1]


      exit(1);
    }

    int PageNumber = PN;
    int FrameNumber = getFrame();
    int Entry = FrameNumber & 0xFFFFFFFE;  // set invalid bit

    Page_Table[Page_Table_count] = Entry;
    Page_Table_count++;
    free_frames_count++;

}

// binary search for our Page Table
int searchPT(int PN){

    int middle, first, last, search, n;
    // binary search
    first = 0;
    last = n - 1;
    middle = (first+last)/2;

    while (first <= last) {
      if (Page_Table[middle] < search)
         first = middle + 1;
      else if (Page_Table[middle] == search) {
         printf("%d found at location %d.\n", search, middle+1);

         return 1;  //HIT
      }
      else
         last = middle - 1;

      middle = (first + last)/2;
   }
   if (first > last)
      printf("Not found! %d is not present in the list.\n", search);

   return 0; // MISS

}



int main(int argc, char *argv[]) {

    FILE *fp;

    fp = fopen("heapsort-trace.bin", "r");
    int *buffer;
    int y;

    int* Page_Table;
    free_frames_count = 0;
    Page_Table_count = 0;

    // QUEUE or FIFO
    front = 0;
    rear = -1;
    itemCount = 0;

    // STACK or LRU

    // int* Page_Table_temp = malloc(200 * sizeof(int));
    // change to dynamic after
    for(y=0; y< 100; y++){
      Page_Table[y]=0;
    }

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


      int shift = address[0] >> 12;
      int offset = address[0] << 20;

      int PageNumber = shift >> 12; //20xbits 10 0's

      //printf("? %d ?", offset);
      offset = offset >> 20;  // ignore



      //printf("%x\n", shift);
      printf("final %04x\n", address[0]);
      printf("PN %d && offset %d\n",shift, offset);
      memset(address, 0, sizeof(address));



    }
    fclose(fp);
    return(0);


}
