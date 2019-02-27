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

// enregister l image de la cle key 
int save(int key){
	

}

//rotation de l image de cle key 
int rotation (int key,short rotation){
	
}

//les autre fonction de traitement de l image independement des fenetres.......








