
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
		if(img_temp->key_image > count){
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
		img->posx = 0; // en haut à gauche par défaut
		img->posy = 0;
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
	img_temp->next->posx = 0;
	img_temp->next->posy = 0;
	img_temp->next->next = NULL;
	img_temp->next->texture = texture;
	img_temp->next->position_texture = position_texture;
	return img_temp->next;
}


//recuperer la structure image de la cle key 
struct image *get_Image_By_Key(struct image* img, int key){
	struct image *img_temp = img;
	while (img_temp != NULL) {
		if (img_temp->key_image == key){
			return img_temp;
		}
		img_temp = img_temp->next;
	}
	return NULL;
}

int blackAndWhite(SDL_Renderer* renderer,SDL_Rect* rectangle, SDL_Texture* texture) {
	int width = rectangle->w;
	int height = rectangle->h;
	void* temp = malloc(width * height * 4);
	int pitch = width*4;
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (SDL_RenderReadPixels(renderer,format,rectangle,&temp,pitch) != 0) {
		printf("\n%s\n",SDL_GetError());
		return -1;
	}
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
	SDL_UpdateTexture(texture,rectangle,pixels,pitch);
	return 0;
}

