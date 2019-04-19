
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <SDL2/SDL_image.h>

#include "image.h"

typedef struct image{
        char *name;
	SDL_Surface *img;
	int type;
} image;

typedef assoc_img{
	int key;
	short exist;
	image *img;
	
} assoc_img;

static assoc_img_tab[MAX_IMG];
static nb;



// lancement du module image au moment du lancement du programme avec initialisation de la table des images 
void init_images(){
	int i;	
	for(i=0;i<MAX_IMG;i++)
		assoc_img_tab[i].exist=false;
	nb=0;
		
} 
//fonction qui stock une image dans le tableau et l associe une cle 
int get_new_key(image *img){
        if(nb<MAX_IMG){
		int i;		
		for(i=0;i<MAX_IMG;i++){
			if(assoc_img_tab[i].exist != 0){
				assoc_img_tab[i]img=img;				
				return i;
			}
		}	
	}
	else 
		return -1;
}

//fonction qui cree une strucrure image pour une nouvelle image et renvoie la cle associe 
int create_img(char *path){
        SDL_Surface *img=NULL;
        img=SDL_loadBMP(path);
	if(img!=NULL)
		return get_new_key(img);
	else 
		return NULL;
}


//recuperer la structure image de la cle key 
image *get_img_by_key(int key){
 
  if(key<MAX_IMG && assoc_img_tab[key].exist)
	return assoc_img_tab[key].img;

  else 
	return -1;
}

/************************************************************************************/

//set a pixel

void setPixel(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a, size_t x, size_t y)
{
    Uint32 *pixels = surface->pixels; /* Nos pixels sont sur 32 bits */
    Uint32 couleur = SDL_MapRGBA(surface->format, r, g, b, a);
    pixels[y * surface->w + x] = couleur;
}


// enregister l image de la cle key retourne -1 en cas d erreur
int save(int key,const char *new_name){
    if (SDL_SaveBMP( assoc_img_tab[key].surface,new_name)< 0)
    {
        fprintf(stderr, "SDL_QueryTexture: %s.\n", SDL_GetError());
	return -1;
    }
    return 0;
	

}



//rotation de l image de cle key 
int rotation (int key,short rotation,SDL_Surface *surface,const SDL_Rect* rect){
	Uint32 *pixels=surface->pixels,*temp;
        int i,j;

	for(i = rect->x; i < rect->x+rect->w; i++)
	{
    		for(j = rect->x; j < rect->x+rect->h; j++)
        		temp[j * surface->h +rect->x + i]=pixels[i * surface->w +rect->x + j];
	} 
	for(i = rect->x; i < rect->x+rect->h; i++)
	{
    		for(j = rect->x; j < rect->x+rect->w; j++)
        		pixels[i * surface->w +rect->x + j]=temp[i * surface->h +rect->x + j];
	} 
	
	surface->pixels=pixels;

		
	return 1;	
}
//les autre fonction de traitement de l image independement des fenetres.......


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

