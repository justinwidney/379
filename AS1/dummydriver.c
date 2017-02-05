#include "findpattern.h"
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]) {	

	char* tpat;

	if (argc !=2 ) { 
	printf("Enter ./driver <pattern> \n");
	return 0;
	}

	//int length = strlen(argv[1]);
	//printf("%c", length);
	int length = 12;

	printf("Enter Pattern \n");
	scanf("%hhu", tpat);


	struct patmatch *pattern = malloc(sizeof(struct patmatch));
	unsigned char* pat = (unsigned char*)tpat;
	//unsigned char* pat = argv[1];
	int var = findpattern(pat, length, pattern, 10);
	printf("Total matches= %d\n", var);
	printf("------------------------------------ \n");
	printf("N--R--W");
	
	/* if(R):
		print("%c", location);
		print("--X--O); */
	
	/* if(W):
		print("%c", loation);
		print("--X--X); */
	
	printf("----------------------------------- \n");
	

	char check[] = "JustinWidney";
	
	var = findpattern(pat, length, pattern, 10);
	printf("Total matches= %d\n", var);
	
}
 
