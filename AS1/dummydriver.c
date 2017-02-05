#include "findpattern.h"
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]) {	

	char* tpat;
	int i;


	//int length = strlen(argv[1]);
	//printf("%c", length);
	//int length = 1;
	//char check[] = "JustinWidney";

	printf("Enter Pattern \n");
	scanf("%hhu", tpat);
	int length = strlen(tpat);

	struct patmatch *pattern = malloc(10 * sizeof(struct patmatch));
	//struct patmatch pattern[10];	
	

	unsigned char* pat = (unsigned char*)tpat;
	//unsigned char* pat = argv[1];
	int var = findpattern(pat, length, pattern, 10);
	printf("Total matches= %d\n", var);
	printf("------------------------------------ \n");
	printf("N--R--W\n");

	for(i = 0; i <10; i++){
	printf("%i, mode = %u\n", pattern[i].location, pattern[i].mode);
	}
	
	/* if(R):
		print("%c", location);
		print("--X--O); */
	
	/* if(W):
		print("%c", loation);
		print("--X--X); */
	
	printf("----------------------------------- \n");
	

	
	
	var = findpattern(pat, length, pattern, 10);
	printf("Total matches= %d\n", var);
	
}
 
