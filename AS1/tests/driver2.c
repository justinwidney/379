#include "../findpattern.h"
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {	
	int i;
	int length;
	int loclength;
	char flag;
	if(argc!=2){printf("\nPlease include a pattern in the command line argument\n"); return 0;}

	char *end;
	char buf[10];
	printf("Enter how many entries to record: ");
	do {
		if(!fgets(buf, sizeof buf, stdin)) break;
		buf[strlen(buf) -1] = 0;
		loclength = strtol(buf, &end, 10);
	} 
	while(end!= buf + strlen(buf));

	length = strlen(argv[1]);
	struct patmatch *pattern = malloc(loclength * sizeof(struct patmatch));
	
	unsigned char* pat = argv[1];

	int var = findpattern(pat, length, pattern, loclength);
	
	printf("test2\n");
	printf("Create a stack variable with the pattern\n\n");
	printf("Pass 1\n");
	printf("Total matches= %d\n", var);
	
	for(i = 0; i < var; i++){
		int unsigned address = pattern[i].location;
		printf("%p\t", (void *)pattern[i].location);
		if(pattern[i].mode = 1) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
		if (i == loclength-1) break;
	}

	// Create a new entry in memory of our pattern
	pat = argv[1];
	length = strlen(argv[1]);
	
	struct patmatch *pattern2 = malloc(loclength * sizeof(struct patmatch));
	char* patternloc;
	patternloc = alloca(sizeof(char) * length);
	patternloc = argv[1];

	// create the pattern on the stack
	char filename[length];
	strcpy(filename, argv[1]);

	int var2 = findpattern(pat, length, pattern2, loclength);
	printf("\nPass 2 \n");
	printf("Total matches = %d\n", var2);
	
	for(i = 0; i < var2; i++){
		flag = 'N';
		int n;
		for(n = 0; n < var; n++) {
			if (pattern2[i].location == pattern[n].location && pattern2[i].mode == pattern[n].mode) {
				flag = 'U'; break;
			}
			else if (pattern2[i].location == pattern[n].location) {
				flag = 'C'; break;
			}
		}
		printf("%p\t", (void *)pattern2[i].location);

		if(pattern2[i].mode == 1) {printf("MEM_RO\t%c\n", flag);} else {printf("MEM_RW\t%c\n", flag);}
		if (i == loclength-1) break;
	}
	return 0;
}
 
