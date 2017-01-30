#include <stdio.h>
#include <signal.h>
#include <stdlib.h>






#define MEM_RW 0
#define MEM_RO 1


struct patmatch {
	unsigned int  location;
	unsigned char mode; /* MEM_RW, or MEM_RO */
};



unsigned int findpattern (unsigned char *pattern, unsigned int patlength,
struct patmatch *locations, unsigned int loclength)


int main(int argc, char ** argv) {


}
