CC = gcc
CFLAGS = -g -Wall -Werror -std=c99

all: fork

fork: fork.c
	$(CC) $(CFLAGS) -o fork fork.c

clean:
	rm -f fork *.o *~

.PHONY: all clean

