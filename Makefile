CC=GCC

CFLAGGS=-Wall -g -std=c11
LDLIBS= -lm -I include/ -L lib/ -l SDL2-2.0.0 -lSDL2_image
ALL =
all : $(ALL)

window : window.o image.o
image.o : image.c image.h
window.o : window.c window.h image.h

cleanall:
	rm -rf *~(ALL)