CC = gcc
CFLAGS = -m32 
OBJS = findpattern.o dummydriver.o

all: driver

driver: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o driver

findpattern.o: findpattern.h
dummydriver.o: findpattern.h

clean:
	rm -f *~ *.o driver
