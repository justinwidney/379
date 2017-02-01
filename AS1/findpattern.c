#include "findpattern.h"

jmp_buf unreadable_memory;
jmp_buf readonly_memory;

char * address = 0x00000000;
char mode = MEM_RO;

/* Handle Segfault
   increment to next page */

void seg_handler(int signo) {
	if (mode == MEM_RO) {
		address += getpagesize();
		siglongjmp(unreadable_memory,1);
	}
	else {
		siglongjmp(readonly_memory,1);
	}
	
}

unsigned int findpattern (unsigned char *pat, unsigned int patlength, struct patmatch *locations, unsigned int loclength) {
struct sigaction seg_act;
	seg_act.sa_handler = seg_handler;
	sigemptyset(&seg_act.sa_mask);
	sigaction(SIGSEGV, &seg_act, NULL); 
	
	int occcurances = 0; 
	char pattern = *pat;
	
	int i = 0;
	while(1) {
		sigsetjmp(readonly_memory,1);
		mode = MEM_RO;
		char * address = 0x0000000;
		mode = MEM_RW;
		char c = *address;
		sigsetjmp(readonly_memory,1);
		address += getpagesize();
		
		if (mode = MEM_RO) { printf("Readable memory at %p\n", address); }
		else { printf("Read and Wrtie memory at %p\n", address); }
		if (i >= 0xFFFFFFFF/getpagesize()) {
			break;
		}
		i++;
	}
	return occcurances;
}
