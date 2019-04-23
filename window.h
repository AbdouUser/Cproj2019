#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#include "image.h"


struct window{
	char* name;
	SDL_Window* pWindow;
	SDL_Renderer* renderer;
	struct image* img_w;
	struct window* next;
};


int init_SDL();
struct window* init_window(char* name, int width, int height);
int add_window(struct window* w, char* name, int width, int height);
struct window* getWindow (struct window* w, char* windowName);

int add_Image_In_Window(struct window* w, SDL_Texture* texture, SDL_Rect* position_texture);
struct image *get_Image_By_Key_In_Window(struct window* w, int key);

void close_window(struct window* w);
void end_SDL();
int wait_event_react_until_quit_or_ask();
int load_An_Image(struct window* w, char* name, char* image);
int move_image(struct window* w, char* name, int img_key, int x_pixels, int y_pixels);
void refreshWindow(struct window* w);
#endif