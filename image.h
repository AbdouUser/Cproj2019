#ifndef IMAGE_H
#define IMAGE_H


struct image{
	int key_image;
	int posx;
	int posy;
	SDL_Texture* texture;
	SDL_Rect* position_texture;
	struct image* next;
};

int get_New_Key(struct image *img);
struct image* add_New_Image(struct image* img, SDL_Texture* texture, SDL_Rect* position_texture);
struct image* get_Image_By_Key(struct image* img, int key);
int blackAndWhite(SDL_Renderer* renderer,SDL_Rect* rectangle, SDL_Texture* texture);



#endif
