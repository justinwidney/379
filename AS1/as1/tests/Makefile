CC = gcc
CFLAGS = -m32

all: driver1 driver2 driver3

test1: driver1.o
	$(CC) $(CFLAGS) $(OBJS) driver1.o -o driver1 && ./driver1 ${p}
test2: driver2.o
	$(CC) $(CFLAGS) $(OBJS) driver2.o -o driver2 && ./driver2 ${p}
test3: driver3.o
	$(CC) $(CFLAGS) $(OBJS) driver3.o -o driver3 && ./driver3 ${p}
	

driver1.o: ../findpattern.h
driver2.o: ../findpattern.h
driver3.o: ../findpattern.h

clean:
	rm -f *~ *.o test
