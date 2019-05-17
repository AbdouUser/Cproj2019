#ifndef USER_COMMANDS_H

#define USER_COMMANDS_H

#include "parser.h"
#include "window.h"


int test1(char* entier);
int test2(char* var, char* entier);
void initCommands(fonction* res, struct window* window, struct image* img);
int NEWWINDOW(char* var, char* entier1, char* entier2);
int LOADIMAGE(char* var, char* fenetre);
int MOVEIMAGE(char* fenetre, char* key, char* posx, char* posy);
int RESIZEIMAGE(char* fenetre, char* key, char* width, char* posy);
int ZOOMWINDOW(char* fenetre, char* zoom);
int REMOVEIMAGE(char* fenetre, char* key);
int FIRSTPLANIMAGE(char* fenetre, char* key);
int GRAYSCALEIMAGE(char* fenetre, char* key);
int FILLWITH(char* fenetre, char* key, char* r, char* g, char* b);
int REPLACEWITH(char* fenetre, char* key, char* r, char* g, char* b, char* interval, char* newr, char* newg, char* newb);
int CREATESELECTION(char* fenetre, char* x1, char* y1, char* x2, char* y2);
int COPY(char* fenetre, char* key);
int CUT(char* fenetre, char* key);
int PASTE(char* fenetre);

#endif