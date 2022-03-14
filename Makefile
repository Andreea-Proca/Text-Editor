CC = gcc
CFLAGS = -g -Wall -lm -O0

all: clean build

build:
	$(CC) -o editor tema1.c $(CFLAGS)
	

.PHONY : clean
clean:
	rm -f editor