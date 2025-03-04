# Darion Kwasnitza
# 3122890
# Makefile

CC = gcc-10
CFLAGS = -Wall -g
TARGET = macD

all: $(TARGET)

$(TARGET): macD.c
	$(CC) $(CFLAGS) -o $(TARGET) macD.c

clean:
	rm -f $(TARGET)

