**************************************
* CMPUT 379 Assignment 1			 *
* By: Justin Widney and Oliver Rarog *
**************************************

Description:
The findpattern function will scan the entire address space of the calling
proccess and given some pattern, it will find the location and addressing
mode of each match. To run the program through some test cases, call 
"make tests p=<pattern>", where <pattern> is some characters or letters.
You will then be prompted to input how many matches you would like to see
and have printed out. To test each case seperatly, call
"make test[1-3]  p=<pattern>". Where [1-3] is the number of the case you
would like to test i.e: "make test1 p=test". After the second call to
findpattern, next to each match there are 1 of 3 flags.
U - Match is unchanged from second call
C - The mode of the location has changed
N - New match found 

Driver Descriptions:

Driver 1: This driver will run through once finding any matches. It will
then place a single instance of the given pattern onto the heap. It will
run through again and should find another match in memory where the pattern
was placed onto the heap.

Driver 2: This driver will will create a local instance of the pattern on
the stack after the first call to findpattern(). This should result in the
function finding another instance of the pattern since it was added onto
the stack.

Driver 3: This driver will create and write the pattern to a file. Then
findpattern is called to find any matches. Then the file will be mapped 
to memory with a different addressing mode at one of the previous matches.
This should result in the program changing the mode of one of the matches
but not the location of the match.

