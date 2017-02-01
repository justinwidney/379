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

	printf("Can't write here! Pointer: %p", address);
	address = address + getpagesize();
	if (address == 0x00000000) {
		exit(0);
	}
	siglongjmp(env,1);
	
	//return;
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

//findpattern 
sigsetjmp(env,1);
//printf(" got out\n");
//printf("ADDRESS:");
//printf("---MODE---\n");
while(1) {
	char tmp = &address;
	//printf("Readable in %p\n", address);
	//*address = 20;
	//printf("Writeable in %p\n", address);
	address += getpagesize();
	if (address == 0x00000000) {
		break;
	}
	//break;
	
	
}

//int temp = &address;
printf("done\n");
}
