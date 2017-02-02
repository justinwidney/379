#include "findpattern.h"
#include <unistd.h>

jmp_buf unreadable_memory;
jmp_buf readonly_memory;

char * address = (char*) 0x00000000;
char mode = MEM_RO;

/* Handle Segfault
   increment to next page */

void seg_handler(int signo) {
	
		siglongjmp(readonly_memory,1);
		return;
}

unsigned int findpattern (unsigned char *pat, unsigned int patlength, struct patmatch *locations, unsigned int loclength) {
	
	// set-up stuff
	struct sigaction seg_act;
	seg_act.sa_handler = seg_handler;
	sigemptyset(&seg_act.sa_mask);
	sigaction(SIGSEGV, &seg_act, NULL); 
	
	int occcurances = 0; 
	char pattern = *pat;
	int sig_value;
	int i = 0;
	int prev_address;
	char data; 
	

	while(1) {
		
		
		sig_value = sigsetjmp(readonly_memory,1);

		if( sig_value == 0) {
		data = *address;
		
		}
		else {
		//printf("Non Readale memory at %p\n", address);
		address += getpagesize();
		continue;
		}

		sig_value = sigsetjmp(readonly_memory,1);

		if(sig_value == 0) {
		
		*address = 'w';
		//printf("why?");
		*address = data; 
	
		mode = MEM_RW;
		//printf("why2?");

		} else {
		mode = MEM_RO;
		//printf("why3?");
		}
		
		// restore stuff 
		//*address = data;
		
		
		if (mode == MEM_RO) { printf("Readable memory at %p\n", address); }
		else if (mode == MEM_RW) { printf("Read and Write memory at %p\n", address); }
		address += getpagesize();

		if(address == 0x00000000){ 
		break;}
	}
	return occcurances;
}
