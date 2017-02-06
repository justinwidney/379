#include "../findpattern.h"
#include <errno.h>

/* Using mmap(2) to change the mode of the memory that we map to*/

int main(int argc, char *argv[]) {
	const char *modes[2] = {"MEM_RW", "MEM_RO"};

	if(argc!=2){printf("\nPlease include a pattern in the command line argument\n"); return 0;}
	char flag;
	char *end;
	char buf[10];
	int loclength;
	printf("Enter how many entries to record: ");
	do {
		if(!fgets(buf, sizeof buf, stdin)) break;
		buf[strlen(buf) -1] = 0;
		loclength = strtol(buf, &end, 10);
	} 
	while(end!= buf +strlen(buf));

	int length = strlen(argv[1]);
	unsigned char *pat = argv[1];
	struct patmatch *pattern = malloc(loclength * sizeof(struct patmatch));
	struct patmatch *pattern2 = malloc(loclength * sizeof(struct patmatch));
	

	FILE *fp = fopen("driver3File.txt", "w+");
	fprintf(fp, "%s", argv[1]); // print pattern once
	
	FILE* fo = fopen("test_results.txt", "a");
	void *filePointer;
	
	filePointer = mmap(0, length, PROT_READ, MAP_PRIVATE, fileno(fp),0);
	fclose(fp);

	int var = findpattern(pat, length, pattern, loclength);
	printf("test3\n");
	fprintf(fo,"\ntest3\n");
	
	printf("using mmap to change the mode of a found location\n\n");
	fprintf(fo,"using mmap to change the mode of a found location\n\n");
	printf("Pass 1\n");
	fprintf(fo,"Pass 1\n");
	fprintf(fo,"Total matches= %d\n", var);
	printf("Total matches= %d\n", var);
	
	int i = 0;
	for(i = 0; i < var; i++){
		printf("%p\t%s\n", (void *)pattern[i].location, modes[pattern[i].mode]);
		fprintf(fo,"%p\t%s\n", (void *)pattern[i].location, modes[pattern[i].mode]);
		if (i == loclength-1) break;
	}
	printf("\n");
	fprintf(fo,"\n");
	
	fp = fopen("driver3File.txt", "r");
	
	filePointer = mmap(filePointer, length, PROT_READ|PROT_WRITE, MAP_PRIVATE| MAP_FIXED, fileno(fp),0);
	
	int var2 = findpattern(pat, length, pattern2, loclength);
	printf("Pass 2\n");
	fprintf(fo,"Pass 2\n");
	printf("Total matches= %d\n", var2);
	fprintf(fo,"Total matches= %d\n", var2);
	
	i = 0;
	for(i = 0; i < var2; i++){
		flag = 'N';
		int n;
		for(n = 0; n < var; n++) {
			if (pattern2[i].location == pattern[n].location && pattern2[i].mode == pattern[n].mode) {
				flag = 'U'; break;
			}
			else if(pattern2[i].location == pattern[n].location) {
				flag = 'C'; break;
			}
		}
		printf("%p\t", (void *)pattern2[i].location);
		fprintf(fo,"%p\t", (void *)pattern2[i].location);

		if(pattern2[i].mode == 1) {printf("MEM_RO\t%c\n", flag);} else {printf("MEM_RW\t%c\n", flag);}
		if(pattern2[i].mode == 1) {fprintf(fo,"MEM_RO\t%c\n", flag);} else {fprintf(fo,"MEM_RW\t%c\n", flag);}
		if (i == loclength-1) break;
	}
	fclose(fp);
	fclose(fo);
	return 0;
	
}
