#include "findpattern.h"

int main(int argc, char ** argv) {
	struct patmatch pattern;
	char pat = 'a';
	int var = findpattern(&pat, 5, &pattern, 10);
	printf("Total matches= %d\n", &var);
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
 
