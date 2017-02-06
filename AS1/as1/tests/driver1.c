#include "../findpattern.h"
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <stdlib.h>





int main(int argc, char *argv[]) {	

	char* tpat;
	int i;
	int length;
	char * p;
	
	

	if(argc!=2){printf(" <pattern> \n"); return 0;}

	length = strlen(argv[1]);
	struct patmatch *pattern = malloc(10 * sizeof(struct patmatch));
	
	char *end;
	char buf[LINE_MAX];
	printf("Enter how many entries to record");
	do {
	if(!fgets(buf, sizeof buf, stdin)) break;
	buf[strlen(buf) -1] = 0;
	int loclength = strol(buf, &end, 10);
	} while(end!= buf +strlen(buf));		

	// put pattern on heap
	unsigned char *firstloc = (char *) malloc(length*sizeof(char));
	unsigned char *secloc = (char*) malloc(length*sizeof(char));
	strcpy(firstloc, argv[1]);
	
	
	unsigned char* pat = argv[1];

	int var = findpattern(pat, length, pattern, loclength);
	
	printf("test1\n");
	printf("heap for pattern, then free it for second call & add new heap pattern\n\n");
	printf("Pass 1\n");
	printf("Total matches= %d\n", var);
	
	
	for(i = 0; i < var; i++){
	int unsigned address = pattern[i].location;
	printf("%p\t", pattern[i].location);
	if(pattern[i].mode = 1) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
	if (i == loclength-1) break;
	}
	
	

	// Create a new entry in memory of our pattern
	length = strlen(argv[1]);
	
	struct patmatch *pattern2 = malloc(10 * sizeof(struct patmatch));

	// remove one heap location and add another
	firstloc(test);
	strcpy(secloc, argv[1]);
	
	char* patternloc;
	patternloc = alloca(sizeof(char) * length);
	patternloc = argv[1];


	int var2 = findpattern(pat, length, pattern2, loclength);
	printf("\nPass 2");
	printf("Total matches= %d\n", var2);
	
	
	for(i = 0; i < var2; i++){
	printf("%p\t", pattern2[i].location);

	if(pattern2[i].mode = 1) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
	if (i == loclength-1) break;
	}
}
 
