#include "findpattern.h"
#include <errno.h>

/* Using mmap(2) to change the mode of the memory that we map to*/

int main(int argc, char *argv[]) {
	const char *modes[2] = {"MEM_RW", "MEM_RO"};
	/********************/
	if(argc!=2){printf("Please include a pattern in the command line argument\n"); return 0;}
	
	int length = strlen(argv[1]);
	unsigned char *pat = argv[1];
	struct patmatch *pass1 = malloc(10 * sizeof(struct patmatch));
	struct patmatch *pass2 = malloc(10 * sizeof(struct patmatch));
	
	int matches = findpattern(pat, length, pass1, 10);
	printf("Total matches= %d\n", matches);
	printf("------------------------------------ \n");
	printf("Address -- Mode \n");
	
	
	int i = 0;
	for(i = 0; i < matches; i++){
		int unsigned address = pass1[i].location;
		unsigned char* hexAddress = (unsigned char*) address;
		printf("%p, %s\n", hexAddress, modes[pass1[i].mode]);
		if (i == 10) break;
	}
	printf("----------------------------------- \n");
	
	// mode of first match will determine if we want to have file be RO or RW
	int mode = pass1[0].mode; 
	FILE *fp = fopen("driver3File.txt", "w");
	fprintf(fp, "%s", argv[1]); // print pattern once

	
	int unsigned address = pass1[0].location;
	unsigned char* hexAddress = (unsigned char*) address;
	
	char *filePointer;
	if(mode == MEM_RO) {
		filePointer = mmap(hexAddress, length, 0, MAP_PRIVATE, fileno(fp), PROT_WRITE);
	}
	else {
		filePointer = mmap(hexAddress, length, 0, MAP_PRIVATE, fileno(fp), PROT_READ);
	}

	printf("%p\n", filePointer);
	printf("Error: %s\n", strerror(errno));
	
	matches = findpattern(pat, length, pass2, 10);
	printf("Total matches= %d\n", matches);
	printf("------------------------------------ \n");
	printf("Address -- Mode \n");
	
	
	i = 0;
	for(i = 0; i < matches; i++){
		int unsigned address = pass2[i].location;
		unsigned char* hexAddress = (unsigned char*) address;
		printf("%p, %s\n", hexAddress, modes[pass2[i].mode]);
		if (i == 10) break;
	}
	printf("----------------------------------- \n");
	
	
	fclose(fp);
	
}
