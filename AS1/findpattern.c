#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>

#define MEM_RW 0
#define MEM_RO 1


jmp_buf env;
char * address = 0x00000000;
struct patmatch {
	unsigned int  location;
	unsigned char mode; /* MEM_RW, or MEM_RO */
};


/* Handle Segfault
   increment to next page */

void seg_handler(int signo) {
	address = address + getpagesize();
	
	//probably don't need this
	if (address == 0x00000000) {
		exit(0);
	}
	siglongjmp(env,1);
}

unsigned int findpattern (unsigned char *pattern, unsigned int patlength,
struct patmatch *locations, unsigned int loclength) {

int occcurances = 0; 


//printf(&address);

while(1){

// try to read 


// try to write

//sigsetjump(env,1);



// set Mode



}


return occcurances;
}


int main(int argc, char ** argv) {

	struct sigaction seg_act;
	seg_act.sa_handler = seg_handler;
	sigemptyset(&seg_act.sa_mask);
	
	sigaction(SIGSEGV, &seg_act, NULL); 
	
	
	//unsigned char* locations[]; 
	
	sigsetjmp(env,1);
	while(1) {
		char tmp = *address;
		printf("Readable page at %p\n", address);
		*address = 20;
		printf("Writeable page at %p\n", address);
		address += getpagesize();
		if (address == 0x00000000) {
			break;
		}
	}
}
