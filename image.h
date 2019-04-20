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
int save(int key,const char *new_name);
int rotation (int key,short rotation,SDL_Surface *surface,const SDL_Rect* rect);
//les autre fonction de traitement de l image independement des fenetres.......
SDL_Surface *createSurfaceFromTexture(SDL_Texture *texture);
SDL_Surface *noirEtBlanc(SDL_Surface *s);

#endif
