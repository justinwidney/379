#include "findpattern.h"

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
	printf("Total matches= %d\n", var);
	printf("------------------------------------ \n");
	printf("Address -- Mode \n");
	
	for(i = 0; i < var; i++){
	int unsigned address = pattern[i].location;
	unsigned char* hexAddress = (unsigned char*) &address;
	printf("%p, %u\n", hexAddress, pattern[i].mode);
	if (i == 10) break;
	}
	
	/* if(R):
		print("%c", location);
		print("--X--O); */
	
	/* if(W):
		print("%c", loation);
		print("--X--X); */
	
	printf("----------------------------------- \n");
	
	sleep(2);
	pat = argv[1];
	length = strlen(argv[1]);
	struct patmatch *pattern2 = malloc(10 * sizeof(struct patmatch));
	char check[] = "Justin";

	int var2 = findpattern(pat, length, pattern2, 10);
	printf("Total matches= %d\n", var2);
	
}
 
