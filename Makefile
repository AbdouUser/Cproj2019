
CC=gcc

CFLAGS=-Wall -g -std=c11 
LDLIBS= -lm -I include/ -L lib/ -l SDL2-2.0.0 -lSDL2_image
all: CIMP


CIMP: main.c parser.c parser.h userCommands.c userCommands.h image.c image.h window.c window.h include/SDL2/SDL.h
	$(CC) *.o -o CIMP -lSDL $(CFLAGS) 

clean:
	rm -rf *.o

mrproper: clean
	rm -rf CIMP
