# name: darion kwasnitza
# id: 3122890
# file: makefile
CC = gcc
CFLAGS = -Wall
TARGET = svm

all: $(TARGET)

$(TARGET): svm.o
	$(CC) $(CFLAGS) -o $(TARGET) svm.o

vm.o: svm.c svm.h
	$(CC) $(CFLAGS) -c svm.c

clean:
	rm -f $(TARGET) svm.o


