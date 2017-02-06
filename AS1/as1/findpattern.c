 #include "findpattern.h"
#include <unistd.h>

jmp_buf unreadable_memory;
jmp_buf readonly_memory;
char data;
int flag =0;
struct sigaction seg_act, seg_act2;
	
/* Handle Segfault
   increment to next page */

void seg_handler(int signo) {
		flag= 1;
		siglongjmp(readonly_memory,1);
}


void restore_handler() {
	sigaction(SIGSEGV, &seg_act2, NULL);
}

unsigned int findpattern (unsigned char *pat, unsigned int patlength, struct patmatch *locations, unsigned int loclength) {
	
	// set-up stuff
	
	seg_act.sa_handler = seg_handler;
	sigemptyset(&seg_act.sa_mask);
	sigaddset(&seg_act.sa_mask, SA_NODEFER);
	sigaction(SIGSEGV, &seg_act, NULL); 
	
	int pagesize = getpagesize();
	
	char * address = (char*) 0x00000000;
	char mode = MEM_RO;
	
	char pattern = *pat;
	int sig_value =0;
	int i = 0;
	int prev_address;
	int count = 0;
	unsigned char bit;
	int occurances =0;

	while(1) { 
		sig_value = sigsetjmp(readonly_memory,1);

		if( flag == 0) {
			data = *address;
		}

		else {
			flag = 0;
			address += pagesize;
		
			if(address == 0x00000000){ 
				break;
			}
			continue;
		}

		flag = 0;
		sig_value = sigsetjmp(readonly_memory,1);

		if(flag == 0) {
			*address = 'w';
			*address = data; 
	
			mode = MEM_RW;
		} 
		else {
			mode = MEM_RO;
		}

		for(i=0; i < pagesize; i++) {
			unsigned char read_pat = *(address + i);
			unsigned char* print_add = address + i;
			bit = *pat;
			
			if( read_pat == bit) {
				pat++;
				bit++;
				count++;
				if(count == patlength){
					if(occurances <= loclength){
						unsigned char* start_add = address + i - patlength;
						locations[occurances].location = start_add;
						locations[occurances].mode = mode;
					}

					occurances++;  
					pat = pat - patlength;
					count = 0;
				}
			}
			else {
				pat = pat - count;
				count = 0;
			}
		}	
		address += pagesize;
		flag = 0;
	}
	restore_handler();
	return occurances;
}
