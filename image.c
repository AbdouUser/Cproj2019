
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <SDL2/SDL_image.h>

#include "image.h"


//fonction qui stock une image dans le tableau et l associe une cle 
int get_New_Key(struct image *img){
	if(img == NULL){ // pas encore d'image
		return 0;
	}
	int count = img->key_image;
	struct image* img_temp = img;
	while(img_temp != NULL){
		if(img_temp->key_image > count) {
			count = img_temp->key_image;
		}
		img_temp = img_temp->next;
	}
	return count + 1;
}

//return the key of the image
struct image* add_New_Image(struct image* img, SDL_Texture* texture, SDL_Rect* position_texture){
	int key = get_New_Key(img);
	if(img == NULL){ // pas encore d'image
		img = malloc(sizeof(struct image));
		if(img == NULL){
			return NULL; //erreur d'allocation
		}
		img->key_image = key;
		img->next = NULL;
		img->texture = texture;
		img->position_texture = position_texture;
		return img; //succès
	}
	struct image* img_temp = img;
	while(img_temp->next != NULL){
		img_temp = img_temp->next;
	}
	img_temp->next = malloc(sizeof(struct image));
	img_temp->next->key_image = key;
	img_temp->next->next = NULL;
	img_temp->next->texture = texture;
	img_temp->next->position_texture = position_texture;
	return img_temp->next;
}


//recuperer la structure image de la cle key 
struct image *get_Image_By_Key(struct image* img, int key){
	struct image *img_temp = img;
	while (img_temp != NULL) {
		if (img_temp->key_image == key) {
			return img_temp;
		}
		img_temp = img_temp->next;
	}
	return NULL;
}

int greyLevels(SDL_Renderer* renderer, SDL_Rect* rectangle, SDL_Texture* texture) {
	int width = rectangle->w;
	int height = rectangle->h;
	void* temp = malloc(width * height * 4);
	//Uint32* pixels = malloc(width * height * 4);
	int pitch = width*4;
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	printf("\nw:%d h:%d x:%d y:%d\n",width,height,rectangle->x,rectangle->y);
	if (renderer == NULL){
		printf("\nRenderer null\n");
		return -1;
	}
	if (SDL_RenderReadPixels(renderer,NULL,0,&temp,pitch) != 0) {
		printf("\n%s\n",SDL_GetError());
		return -1;
	}
	printf("\nw:%d h:%d x:%d y:%d\n",width,height,rectangle->x,rectangle->y);
	
	Uint8 r,g,b,a;
	Uint32* pixels = temp;
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			SDL_GetRGBA(pixels[i* width +j], format,&r,&g,&b,&a);
			Uint8 gris = (r + g + b) / 3;
			pixels[i* width +j] = SDL_MapRGBA(format,gris,gris,gris,a);
		}
	}
	SDL_FreeFormat(format);
	if (SDL_UpdateTexture(texture,rectangle,pixels,pitch) != 0) {
		SDL_GetError();
	}
	return 0;
}

Uint32 couleur(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
   return r << 24 | g << 16 | b << 8 | a;
}

int greyLevelsbis(SDL_Renderer* renderer,SDL_Rect* rectangle, SDL_Texture* texture) {
	
	int pitch = 0;
	int format;
	int w = rectangle->w;
	int h = rectangle->h;
	Uint32* pixels = malloc(4*w*h);
	/*
	if (SDL_QueryTexture(texture, &format, NULL,&w,&h) != 0) {
		SDL_GetError();
	}
	*/
	if (SDL_LockTexture(texture , NULL, (void**)pixels,&pitch) != 0){
		SDL_GetError();
	}
	
	//Uint8 r,g,b,a;
	printf("\nw:%d h:%d\n",w,h);
	for (size_t i = 0; i < h; i++) {
		for (size_t j = 0; j < w; j++) {
			//printf("\ni:%d j:%d\n",i,j);
			//SDL_GetRGBA(pixels[i* w +j], &pFormat,&r,&g,&b,&a);
			//printf("\ni:%d j:%d\n",i,j);
			//Uint8 gris = (r + g + b) / 3;
			pixels[i* w +j] = couleur(0,0,0,255);
			//printf("\ni:%d j:%d\n",i,j);
		}
	}
	if (SDL_UpdateTexture(texture,rectangle,pixels,pitch) != 0) {
		SDL_GetError();
	}
	SDL_UnlockTexture(texture);
	return 0;
}