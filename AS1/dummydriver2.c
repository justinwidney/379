#include "findpattern.h"


/* Other driver program */ 
/* change location and chage mode*/



//void *mmap( void *addr, size_t length int prot int flash, int fd, off_t offest);

int main(int argc, char ** argv) {
	struct patmatch pattern;
	char pat = 'a';
	int var = findpattern(&pat, 5, &pattern, 10);
	printf("Total matches= %d\n", &var);
	
	void *nmap( void *addr, size_t length int prot int flash, int fd, off_t offest);

}
