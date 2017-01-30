#include <stdio.h>
#include <signal.h>
#include <stdlib.h>






#define MEM_RW 0
#define MEM_RO 1


jmp_buf env;

struct patmatch {
	unsigned int  location;
	unsigned char mode; /* MEM_RW, or MEM_RO */
};


/* Handle Segfault
   increment to next page */

void seg_handler(int signo) {


	siglongjmp(env,1);
	address = address + PAGE_SIZE;
}

unsigned int findpattern (unsigned char *pattern, unsigned int patlength,
struct patmatch *locations, unsigned int loclength) {

int occcurances = 0 ; 

void * address = 0x00000000
print(&address)

while(1){

// try to read 


// try to write

sigsetjump(env,1);



// set Mode



}


return occurances
}


int main(int argc, char ** argv) {

struct sigaction seg_act;
seg_act.sa_handler = seg_Handler;
sigemptyset(&seg_act.sa_mask);

sigaction(SIGSEGV, &seg_act, NULL);


unsigned char* locations[]; 

findpattern

printf("ADDRESS:")
printf("---MODE---")




}
