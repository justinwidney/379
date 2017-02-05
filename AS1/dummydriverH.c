#include "findpattern.h"
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <stdlib.h>



void heap() {

	

}




int main(int argc, char *argv[]) {	

	char* tpat;
	int i;
	int length;
	char * p;

	//int length = strlen(argv[1]);
	//printf("%c", length);
	//int length = 1;
	//char check[] = "JustinWidney";

	if(argc!=3){printf(" <pattern> < int loclength>\n"); return 0;}

	//printf("Enter Pattern \n");
	//scanf("%hhu", tpat);
	//int length = 1;
	length = strlen(argv[1]);
	struct patmatch *pattern = malloc(10 * sizeof(struct patmatch));
	//struct patmatch pattern[10];	
	

	unsigned char *test = (char *) malloc(6*sizeof(char));
	strcpy(test, "justin");
	
	//unsigned char* pat = (unsigned char*)tpat;
	unsigned char* pat = argv[1];
	unsigned int loclength = strtol(argv[2], &p, 10);

	
	//unsigned char* pat = argv[1];
	int var = findpattern(pat, length, pattern, loclength);
	
	printf("test1\n");
	printf("Malloc space on heap for pattern, then free it for second call\n\n");
	printf("Pass 1\n");
	printf("Total matches= %d\n", var);
	//printf("Address -- Mode \n");
	
	for(i = 0; i < var; i++){
	int unsigned address = pattern[i].location;
	//unsigned char* hexAddress = (unsigned char*) &address;
	printf("%p\t", pattern[i].location);
	if(pattern[i].mode = 1) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
	if (i == loclength-1) break;
	}
	
	/* if(R):
		print("%c", location);
		print("--X--O); */
	
	/* if(W):
		print("%c", loation);
		print("--X--X); */
	
	printf("----------------------------------- \n");
	

	// Create a new entry in memory of our pattern
	pat = argv[1];
	length = strlen(argv[1]);
	
	//free(test);
	//memset(test,0,loclength*sizeof(char));

	struct patmatch *pattern2 = malloc(10 * sizeof(struct patmatch));

	//char *patternloc;
	//patternloc = malloc(sizeof(char) * 6);
	//patternloc[0] = "justin";
	
	free(test);
	
	char* patternloc;
	patternloc = alloca(sizeof(char) * length);
	patternloc = argv[1];

	//test = (char*) realloc(test,6*sizeof(char));
	//strcat(test,"xyz");

	//char filename[length];
	//strcpy(filename, argv[1]);

	int var2 = findpattern(pat, length, pattern2, loclength);
	printf("Total matches= %d\n", var2);
	
	//printf("Address -- Mode \n");
	
	for(i = 0; i < var2; i++){
	//int unsigned address = pattern2[i].location;
	//unsigned char* hexAddress = (unsigned char*) &address;
	printf("%p\t", pattern2[i].location);

	if(pattern2[i].mode = 1) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
	if (i == loclength-1) break;
	}
	
	printf("------------------------------------ \n");
	//free(test);
}
 
