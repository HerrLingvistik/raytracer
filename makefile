#This is a comment, we save g++ in a variable CC.
CC=gcc -g
#Another comment, a flag giving all them warnings 
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) -o strut utils/*.c camera.c -lm

