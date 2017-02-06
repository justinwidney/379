CC = gcc
CFLAGS = -m32
OBJS = $(CURDIR)/findpattern.o
export OBJS

tests: findpattern.o test1 test2 test3

test1:
	$(MAKE) test1 -C tests
test2:
	$(MAKE) test2 -C tests
test3:
	$(MAKE) test3 -C tests

findpattern.o: findpattern.h

clean:
	rm -f *~ *.o test
