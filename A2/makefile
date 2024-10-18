CC = gcc
CFLAGS = -Wall

all: binscan

binscan: binscan.o
	$(CC) -o binscan binscan.o

binscan.o: binscan.c binscan.h
	$(CC) $(CFLAGS) -c binscan.c

clean:
	rm -f binscan *.o


