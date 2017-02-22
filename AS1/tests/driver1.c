#include "../findpattern.h"
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {	
	char flag;
	int i;
	int length;
	int loclength;
	
	if(argc!=2){printf(" <pattern> \n"); return 0;}

	length = strlen(argv[1]);
	struct patmatch *pattern = malloc(10 * sizeof(struct patmatch));
	
	char *end;
	char buf[10];
	printf("Enter how many entries to record: ");
	do {
		if(!fgets(buf, sizeof buf, stdin)) break;
		buf[strlen(buf) -1] = 0;
		loclength = strtol(buf, &end, 10);
	} 
	while(end!= buf +strlen(buf));		

	// put pattern on heap
	unsigned char *secloc = (char*) malloc(length*sizeof(char));
	
	unsigned char* pat = argv[1];

	int var = findpattern(pat, length, pattern, loclength);
	
	FILE* fp = fopen("test_results.txt", "w");
	
	printf("test1\n");
	fprintf(fp,"test1\n");

	printf("heap for pattern, for second call add new heap pattern\n\n");
	fprintf(fp,"heap for pattern, for second call add new heap pattern\n\n");

	printf("Pass 1\n");
	fprintf(fp,"Pass 1\n");

	printf("Total matches= %d\n", var);
	fprintf(fp,"Total matches= %d\n", var);
	
	
	for(i = 0; i < var; i++){
		int unsigned address = pattern[i].location;
		printf("%p\t", (void *) pattern[i].location);
		fprintf(fp,"%p\t", (void *) pattern[i].location);
		if(pattern[i].mode = 1) {printf("MEM_RO\n"); fprintf(fp,"MEM_RO\n");  } else {printf("MEM_RW\n"); fprintf(fp,"MEM_RW\n");}
		if (i == loclength-1) break;
	}
	
	// Create a new entry in memory of our pattern
	length = strlen(argv[1]);
	
	struct patmatch *pattern2 = malloc(10 * sizeof(struct patmatch));

	// remove one heap location and add another
	strcpy(secloc, argv[1]);
	

	int var2 = findpattern(pat, length, pattern2, loclength);
	printf("\nPass 2\n");
	fprintf(fp,"\nPass 2\n");
	
	printf("Total matches= %d\n", var2);
	fprintf(fp,"Total matches= %d\n", var2);
	
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
		fprintf(fp,"%p\t", (void *)pattern2[i].location);

		if(pattern2[i].mode == 1) {printf("MEM_RO\t%c\n", flag); fprintf(fp,"MEM_RO\t%c\n", flag);} else {printf("MEM_RW\t%c\n", flag); fprintf(fp,"MEM_RW\t%c\n", flag);}
		if (i == loclength-1) break;
	}
	fclose(fp);
	free(secloc);
	return 0;
}
 
