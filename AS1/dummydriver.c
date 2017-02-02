#include "findpattern.h"

int main(int argc, char ** argv) {	

	struct patmatch pattern;
	unsigned char* pat = (unsigned char*) "JuStinWidney";
	int var = findpattern(pat, 12, &pattern, 10);
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
	


	
}
 
