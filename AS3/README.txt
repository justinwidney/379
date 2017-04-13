**************************************
* CMPUT 379 Assignment 3			       *
* By: Justin Widney and Oliver Rarog *
**************************************



Overview:

Three data structures:

Hash table for TLB,
Double linked list for PageTable and for LRU/FIFO
Binary Search tree for traversing to PageTable given Page Number
Struct for Holding files, that rotates every time it iterates through quantom bytes

Binary Search tree and hash were to implement efficient lookups.
While with pointers to head and the tail of our double linked list. We could do LRU and
FIFO easily.

For scalable we can realloc our PageTable && BinarySearch Tree every quantom lookups.




