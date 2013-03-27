#
# Makefile for database module
#
include ../Make.public

all: mutex.o condition.o XMLScanner.o Sem.o Common.o

$(DEPS):
	$(CC) -c $^ $(CPPFLAGS)

clean:
	rm -f *.o
