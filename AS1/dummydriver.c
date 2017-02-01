#include "findpattern.h"

int main(int argc, char ** argv) {
	struct patmatch pattern;
	char pat = 'a';
	int var = findpattern(&pat, 5, &pattern, 10);
	printf("done!\n");
}
 
