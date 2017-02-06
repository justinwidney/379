#ifndef FIND_PATTERN_H
#define FIND_PATTERN_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define MEM_RW 0
#define MEM_RO 1

struct patmatch {
	unsigned int  location;
	unsigned char mode; /* MEM_RW, or MEM_RO */
};

unsigned int findpattern (unsigned char *pat, unsigned int patlength,
struct patmatch *locations, unsigned int loclength);

#endif
