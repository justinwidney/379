#include "findpattern.h"
#include <unistd.h>

jmp_buf unreadable_memory;
jmp_buf readonly_memory;

char * address = (char*) 0x00000000;
char mode = MEM_RO;
char data; 
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
	
	int pagesize = getpagesize();
	
	int *ip;
	char pattern = *pat;
	//unsigned char *spat = *pat;
	int sig_value;
	int i = 0;
	int prev_address;
	
	unsigned char bit;
	int count;

	printf("test");
	
	// program crashes if not here ??? 
	int *ptr = (int *)malloc(1*sizeof(int));
	//*ptr = 0;


	int occurances =0;

	while(1) {
		
		

		sig_value = sigsetjmp(readonly_memory,1);

		if( sig_value == 0) {
		data = *address;

		
		}
		else {
		//printf("Non Readale memory at %p\n", address);
		address += pagesize;
		
		if(address == 0x00000000){ 
		break;}

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
		
		//printf("Address = %p", address);
		for(i=0; i < pagesize; i++) {
		unsigned char read_pat = *(address + i);
		
		//printf("%d", read_pat);
		

		bit = *pat;
		
		


		if( read_pat == bit) {
		
		//printf(" bit = %i pat = %i, %d\n", read_pat, bit, count);
		//sleep(1);

		pat++;
		//printf("working... %c\n", bit);
		//sleep(1);
		bit++;
		count++;
		
		if(count > 10) { }

			if(count == patlength){
			//printf("wok");
			occurances++;  
			pat = pat - patlength;
			count = 0;
			}

		
		}

		else{
		pat = pat - count;
		count = 0;

		}
			
		//else{
		//*pat = *spat;
		//} 

		// check character = pattern
			// pattern +i
		// else pattern[x] = 0		

		
		}	
		
		//if (mode == MEM_RO) { printf("Readable memory at %p\n", address); }
		//else if (mode == MEM_RW) { printf("Read and Write memory at %p\n", address); }
		address += pagesize;

		
	}
	return occurances;
}
