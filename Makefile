all: CIMP

CIMP: main.c parser.c parser.h userCommands.c userCommands.h
	gcc -o CIMP *.c -Wall -std=c11

clean:
	rm -rf *.o

mrproper: clean
	rm -rf CIMP
