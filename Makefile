CC = gcc
CFLAGS = -Wall -std=c11
LDFLAGS = -lSDL2 -lSDL2_image
ALL = CIMP
all : $(ALL)

$(ALL) : main.o window.o image.o parser.o userCommands.o
	$(CC) $(CFLAGS) *.o $(LDFLAGS) -o $(ALL)
main.o : main.c userCommands.h parser.h window.h
userCommands.o : userCommands.c userCommands.h window.h
parser.o : parser.c parser.h
image.o : image.c image.h
window.o : window.c window.h image.h

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(ALL)
