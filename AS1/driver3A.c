#include "findpattern.h"
#include <errno.h>

/* Using mmap(2) to change the mode of the memory that we map to*/

int main(int argc, char *argv[]) {
	const char *modes[2] = {"MEM_RW", "MEM_RO"};
	/********************/
	if(argc!=3){printf("Please include a pattern,loclength in the command line argument\n"); return 0;}
	unsigned int loclength = strtol(argv[2], &p, 10);
	int length = strlen(argv[1]);
	unsigned char *pat = argv[1];
	struct patmatch *pass1 = malloc(10 * sizeof(struct patmatch));
	struct patmatch *pass2 = malloc(10 * sizeof(struct patmatch));
	

	FILE *fp = fopen("driver3File.txt", "w+");
	fprintf(fp, "%s", argv[1]); // print pattern once

	
	
	void *filePointer;
	
	filePointer = mmap(0, length, PROT_READ, MAP_PRIVATE, fileno(fp),0);
	//printf("%p\n", filePointer);
	//printf("Error: %s\n", strerror(errno));

	fclose(fp);

	int matches = findpattern(pat, length, pass1, loclength);
	printf("test3\n");
	printf("using mmap to create a MEM_RO pattern then change it to MEM_RW\n\n");
	printf("Pass 1");
	printf("Total matches= %d\n", matches);
	
	
	
	int i = 0;
	for(i = 0; i < matches; i++){
		//int unsigned address = pass1[i].location;
		//unsigned char* hexAddress = (unsigned char*) address;
		printf("%p, %s\n", pass1[i].location, modes[pass1[i].mode]);
		if (i == 10) break;
	}
	printf("\n");
	
	fp = fopen("driver3File.txt", "r");
	
	filePointer = mmap(filePointer, length, PROT_READ|PROT_WRITE, MAP_PRIVATE| MAP_FIXED, fileno(fp),0);
	
	matches = findpattern(pat, length, pass2, loclength);
	prinft("Pass 2\n");
	printf("Total matches= %d\n", matches);
	
	
	i = 0;
	for(i = 0; i < matches; i++){
		int unsigned address = pass2[i].location;
		unsigned char* hexAddress = (unsigned char*) address;
		printf("%p, %s\n", hexAddress, modes[pass2[i].mode]);
		if (i == 10) break;
	}

	
	
	
	
}
