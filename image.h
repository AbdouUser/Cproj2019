#ifndef IMAGE_H
#define IMAGE_H


struct image{
	int key_image;
	SDL_Texture* texture;
	SDL_Surface* surface;
	SDL_Rect* position_texture;
	struct image* next;
};

int get_New_Key(struct image *img);
struct image* add_New_Image(struct image* img, SDL_Texture* texture, SDL_Rect* position_texture, SDL_Surface* surface);
struct image* get_Image_By_Key(struct image* img, int key);
void set_size(struct image* img, int width, int height);
void zoom_on_a_image(struct image* img, double zoom);

#endif
