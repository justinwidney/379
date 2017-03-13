CC = gcc
CFLAGS = -pthread -m32

all: wbc379 wbs379

wbc379: wbc379.c
	$(CC) $(CFLAGS) wbc379.c -o wbc379 -lcrypto

wbs379: wbs379.c
	$(CC) $(CFLAGS) wbs379.c -o wbs379

wbc379.o: findpattern.h
wbs379.o: findpattern.h

clean:
	rm -f *~ *.o wbs379 wbc379
