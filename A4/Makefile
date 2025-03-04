# Name: Darion Kwasnitza
# ID: 3122890
# File: Makefile

CC = gcc
CFLAGS = -pthread -Wall 
TARGET = m6


all: $(TARGET)


$(TARGET): m6.c m6.h
	$(CC) $(CFLAGS) m6.c -o $(TARGET) -lm


clean:
	rm -f $(TARGET)

