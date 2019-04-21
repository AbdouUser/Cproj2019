
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
	int count = img->key_image + 1;
	struct image* img_temp = img;
	while(img_temp->next != NULL){
		img_temp = img_temp->next;
		if(img_temp->key_image >= count){
			count = img_temp->key_image + 1;
		}
	}
	return count;
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
	return img_temp;
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

/************************************************************************************/

//set a pixel

void setPixel(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a, size_t x, size_t y)
{
    Uint32 *pixels = surface->pixels; /* Nos pixels sont sur 32 bits */
    Uint32 couleur = SDL_MapRGBA(surface->format, r, g, b, a);
    pixels[y * surface->w + x] = couleur;
}

//A corriger pour adapter a la struct image du fichier window, on a plus une surface mais une texture.
/*
// enregister l image de la cle key retourne -1 en cas d erreur
int save(int key,const char *new_name) {
    if (SDL_SaveBMP( assoc_img_tab[key]->surface,new_name)< 0) {
      fprintf(stderr, "SDL_QueryTexture: %s.\n", SDL_GetError());
			return -1;
    }
    return 0;
}
*/
//rotation de l image de cle key 
int rotation (int key,short rotation,SDL_Surface *surface,const SDL_Rect* rect){
	Uint32 *pixels=surface->pixels,*temp;
  int i,j;
	for(i = rect->x; i < rect->x+rect->w; i++) {
    		for(j = rect->x; j < rect->x+rect->h; j++)
        		temp[j * surface->h +rect->x + i]=pixels[i * surface->w +rect->x + j];
	} 
	for(i = rect->x; i < rect->x+rect->h; i++) {
    		for(j = rect->x; j < rect->x+rect->w; j++)
        		pixels[i * surface->w +rect->x + j]=temp[i * surface->h +rect->x + j];
	} 
	
	surface->pixels=pixels;

		
	return 1;	
}

//les autre fonction de traitement de l image independement des fenetres

SDL_Surface *createSurfaceFromTexture(SDL_Texture *texture) {
    Uint32 format_pixels;
    SDL_Surface *surface = NULL;
    void *pixels = NULL;
    int pitch, w, h;

    if (SDL_QueryTexture(texture, &format_pixels, NULL, &w, &h) != 0) {
        fprintf(stderr, "SDL_QueryTexture: %s.\n", SDL_GetError());
        goto query_texture_fail;
    }

    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
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

SDL_Surface *noirEtBlanc(SDL_Surface *s) {
	SDL_Surface *retour = NULL;
	size_t i, j, w = (size_t) s->w, h = (size_t) s->h;
	Uint32 *dst = NULL, *src = NULL;
	if(SDL_LockSurface(s) < 0) {
			fprintf(stderr, "Erreur SDL_LockSurface : %s", SDL_GetError());
			goto lock_surface_fail;
	}
	dst = malloc((size_t)s->pitch * h);
	if(NULL == dst) {
			perror("Erreur malloc : ");
			goto alloc_memory_fail;
	}
	src = (Uint32 *)s->pixels;
	SDL_Color color;
	for(i = 0; i < h; i++) {
			for(j = 0; j < w; j++){
					SDL_GetRGB(src[i * w *+ j], s->format,&color.r,&color.g,color.b);
					Uint8 gris = (color.r + color.g + color.b) / 3;
					dst[i*w+j] = SDL_MapRGB(s->format->format,gris,gris,gris);
			}
	}
	retour = SDL_CreateRGBSurfaceWithFormatFrom(dst, s->w, s->h, 32, s->pitch,s->format->format);
	if(NULL == retour) {
			fprintf(stderr, "Erreur SDL_CreateRGBSurface : %s", SDL_GetError());
			goto creatergbsurfacefrom_fail;
	}
	creatergbsurfacefrom_fail:
			free(dst);
	alloc_memory_fail:
			SDL_UnlockSurface(s);
	lock_surface_fail:
			return retour;
}

