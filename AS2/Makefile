CC = gcc
CFLAGS = -m32 

all: client server

client: wbc379.c
	$(CC) $(CFLAGS) wbc379.c -o client

server: wbs379.c
	$(CC) $(CFLAGS) wbs379.c -o server

wbc379.o: findpattern.h
wbs379.o: findpattern.h

clean:
	rm -f *~ *.o driver
