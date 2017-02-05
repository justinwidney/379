CC = gcc
CFLAGS = -m32 
OBJS = findpattern.o driver3.o

all: driver

driver: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o driver

findpattern.o: findpattern.h
driver3.o: findpattern.h

clean:
	rm -f *~ *.o driver
