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

int add_Image_In_Window(struct window* w, SDL_Texture* texture, SDL_Rect* position_texture, SDL_Surface* surface);
struct image *get_Image_By_Key_In_Window(struct window* w, int key);

void close_window(struct window* w);
void end_SDL();
int wait_event_react_until_quit_or_ask();
int load_An_Image(struct window* w, char* name, char* image);
int move_image(struct window* w, char* name, int img_key, int x_pixels, int y_pixels);
void refreshWindow(struct window* w);
int resize_image(struct window* w, char* name_w, int img_key, int width, int height);
int zoom_image(struct window* w, char* name_w, int img_key, double zoom);
int zoom_window(struct window* w, char* name_w, double zoom);
int remove_image(struct window* w, char* name_w, int img_key);
int image_to_first_plan(struct window* w, char* name_w, int img_key);
int image_to_grayscale(struct window* w, char* name_w, int img_key);
int fill_with_color(struct window* w, char* name_w, int img_key, SDL_Color c);
int replace_with_color(struct window* w, char* name_w, int img_key, Uint8 r, Uint8 g, Uint8 b,Uint8 interval, Uint8 newr, Uint8 newg, Uint8 newb);

#endif