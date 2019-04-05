CC=GCC

CFLAGGS=-Wall -g -std=c11
LDLIBS= -lm -I include/ -L lib/ -l SDL2-2.0.0 -lSDL2_image
ALL = main
all : $(ALL)

main : main.o window.o image.o userCommands.o parser.o
main.o : main.c window.h userCommands.h parser.h main.h
userCommands.o : userCommands.c userCommands.h main.h window.h
parser.o : parser.c parser.h
image.o : image.c image.h
window.o : window.c window.h image.h

cleanall:
	rm -rf *~(ALL)