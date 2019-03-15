#include <SDL2/SDL.h>
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

typedef struct assoc_img{
	int key;
	short exist;
	image *img;
} assoc_img;

static assoc_img assoc_img_tab[MAX_IMG];
static int nb;



// lancement du module image au moment du lancement du programme avec initialisation de la table des images 
void init_images(){
	int i;	
	for(i=0;i<MAX_IMG;i++)
		assoc_img_tab[i].exist=0;
	nb=0;
		
} 

//fonction qui stock une image dans le tableau et l associe une cle 
int get_new_key(image *img){
	return 1;
}

//fonction qui cree une strucrure image pour une nouvelle image et renvoie la cle associe 
int create_img(char *path){ /***d autre parametre a propose***/
        SDL_Surface *img=NULL;
        img=IMG_Load(path);
        image* i = malloc(sizeof(image)); 
        i->img = img;
        //gestion d une eventuelle erreur....
	
		return get_new_key(i);
}


//recuperer la structure image de la cle key 
image *get_img_by_key(int key){
 
  if(key<MAX_IMG && assoc_img_tab[key].exist)
	return assoc_img_tab[key].img;

  else
	return NULL;
}

// enregister l image de la cle key 
int save(int key){
	return 1;
}

//rotation de l image de cle key 
	int rotation (){ /*** d autre parametres ....*/
		return 1;
	}


//les autre fonction de traitement de l image independement des fenetres.......
	//reourner -1 ou/et message d erreur.
}

// enregister l image de la cle key 
int save(int key){return 1;}

//rotation de l image de cle key 
int rotation (int key){
  return 1;
}

//les autre fonction de traitement de l image independement des fenetres.......

