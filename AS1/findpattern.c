#include "findpattern.h"

jmp_buf env;
char * address = 0x00000000;

/* Handle Segfault
   increment to next page */

void seg_handler(int signo) {
	address = address + getpagesize();
	if (address == 0x00000000) {
		exit(0);
	}
	siglongjmp(env,1);
}

unsigned int findpattern (unsigned char *pattern, unsigned int patlength, struct patmatch *locations, unsigned int loclength) {
struct sigaction seg_act;
	seg_act.sa_handler = seg_handler;
	sigemptyset(&seg_act.sa_mask);
	sigaction(SIGSEGV, &seg_act, NULL); 
	
	int occcurances = 0; 
		
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
	return occcurances;
}
