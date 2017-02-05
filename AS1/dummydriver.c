#include "findpattern.h"
#include <unistd.h>
#include <string.h>
#include <alloca.h>







int main(int argc, char *argv[]) {	

	char* tpat;
	int i;
	int length;

	//int length = strlen(argv[1]);
	//printf("%c", length);
	//int length = 1;
	//char check[] = "JustinWidney";

	if(argc!=2){printf(" <pattern>"); return 0;}

	//printf("Enter Pattern \n");
	//scanf("%hhu", tpat);
	//int length = 1;
	length = strlen(argv[1]);
	struct patmatch *pattern = malloc(10 * sizeof(struct patmatch));
	//struct patmatch pattern[10];	
	

	//unsigned char* pat = (unsigned char*)tpat;
	unsigned char* pat = argv[1];
	
	//unsigned char* pat = argv[1];
	int var = findpattern(pat, length, pattern, 10);
	printf("------------------------------------ \n");
	printf("Total matches= %d\n", var);
	printf("Address -- Mode \n");
	
	for(i = 0; i < var; i++){
	int unsigned address = pattern[i].location;
	//unsigned char* hexAddress = (unsigned char*) &address;
	printf("%p, ", pattern[i].location);
	if(pattern[i].mode = 0) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
	if (i == 10) break;
	}
	
	/* if(R):
		print("%c", location);
		print("--X--O); */
	
	/* if(W):
		print("%c", loation);
		print("--X--X); */
	
	printf("----------------------------------- \n");
	
	pat = argv[1];
	length = strlen(argv[1]);
	struct patmatch *pattern2 = malloc(10 * sizeof(struct patmatch));

	//char *patternloc;
	//patternloc = malloc(sizeof(char) * 6);
	//patternloc[0] = "justin";

	
	char* patternloc;
	patternloc = alloca(sizeof(char) * length);
	patternloc = argv[1];


	int var2 = findpattern(pat, length, pattern2, 10);
	printf("Total matches= %d\n", var2);
	
	printf("Address -- Mode \n");
	
	for(i = 0; i < var2; i++){
	//int unsigned address = pattern2[i].location;
	//unsigned char* hexAddress = (unsigned char*) &address;
	printf("%p, ", pattern2[i].location);

	if(pattern2[i].mode = 0) {printf("MEM_RO\n");} else {printf("MEM_RW\n");}
	if (i == 10) break;
	}
	
	printf("------------------------------------ \n");
}
 
