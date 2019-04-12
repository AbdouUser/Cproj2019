#ifndef USER_COMMANDS_H

#define USER_COMMANDS_H

#include "parser.h"
#include "window.h"


int test1(char* entier);
int test2(char* var, char* entier);
void initCommands(fonction* res, struct window* w);
int NEWWINDOW(char* var, char* entier1, char* entier2);
int LOADIMAGE(char* var, char* fenetre);

#endif