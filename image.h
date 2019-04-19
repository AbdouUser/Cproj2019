#ifndef IMAGE_H
#define IMAGE_H

struct image;
typedef struct image image;

#define MAX_IMG 1000 

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

void init_images();
int get_new_key(image *img);

int create_img(char *path); /***d autre parametre a propose***/
image *get_img_by_key(int key);
int save(int key);
int rotation (int key); /*** d autre parametres ....*/
//les autre fonction de traitement de l image independement des fenetres.......
SDL_Surface *noirEtBlanc(SDL_Surface *s);

#endif
