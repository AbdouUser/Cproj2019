
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
struct image* add_New_Image(struct image* img, SDL_Texture* texture, SDL_Rect* position_texture, SDL_Surface* surface){
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
		img->surface = surface;
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
	img_temp->next->surface = surface;
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

//change la taille d'une image donnée
void set_size(struct image* img, int width, int height){
	img->position_texture->w = width;
	img->position_texture->h = height;
}

//applique un facteur de zoom à une image donée
void zoom_on_a_image(struct image* img, double zoom){
	img->position_texture->w = (int)(((double)img->position_texture->w)*zoom);
	img->position_texture->h = (int)(((double)img->position_texture->h)*zoom);
}



SDL_Surface *createSurfaceFromTexture(SDL_Texture *texture)
{
    Uint32 format_pixels;
    SDL_Surface *surface = NULL;
    void *pixels = NULL;
    int pitch, w, h;

    if (SDL_QueryTexture(texture, &format_pixels, NULL, &w, &h) != 0)
    {
        fprintf(stderr, "SDL_QueryTexture: %s.\n", SDL_GetError());
        goto query_texture_fail;
    }

    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0)
    {
        fprintf(stderr, "SDL_LockTexture: %s.\n", SDL_GetError());
        goto lock_texture_fail;
    }

    surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 32, w * sizeof(Uint32),format_pixels);
    if(NULL == surface)
        fprintf(stderr, "Erreur SDL_CreateSurfaceFrom : %s.\n", SDL_GetError());

    SDL_UnlockTexture(texture);
lock_texture_fail:
query_texture_fail:
    return surface;
}











