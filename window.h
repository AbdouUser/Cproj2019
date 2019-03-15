#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#include "image.h"

struct image_window;
struct window;

int init_SDL();
struct window* init_window(char* name, int width, int height);
int add_window(struct window* w, char* name, int width, int height);
void close_window(struct window* w);
void end_SDL();
int wait_event_react_until_quit_or_ask();

#endif