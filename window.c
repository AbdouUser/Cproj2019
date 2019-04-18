#include <SDL2/SDL.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "image.h"

struct image_window{
	int key_image;
	int posx;
	int posy;
	SDL_Texture* texture;
	SDL_Rect* position_texture;
	struct image_window* next;
};

struct window{
	char* name;
	SDL_Window* pWindow;
	SDL_Renderer* renderer;
	struct image_window* img_w;
	struct window* next;
};

//initie la SDL, return 0 si initialisation réussie -1 sinon.
int init_SDL(){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		return -1;
	}
	if(SDL_VideoInit(NULL) !=0){
		return -1;
	}
	return 0;
}

//initie une liste chainée de fenêtre avec une premiere fenetre de nom name, et de taille width x height, return NULL en cas d'échec
struct window* init_window(char* name, int width, int height){
	if(name == NULL || width <=0 || height <= 0){
		return NULL;
	}
	struct window* w = malloc(sizeof(struct window));
	w->name = malloc(strlen(name));
	w->name = strcat(w->name, name);
	w->next = NULL;
	w->img_w = NULL;
	w->pWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(w->pWindow == NULL){
		return NULL;
	}
	SDL_SetWindowTitle(w->pWindow, name);
	w->renderer = SDL_CreateRenderer(w->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(w->renderer == NULL){
		return NULL;
	}
	//on définit la couleur de fond de la fenetre (gris)
	if(SDL_SetRenderDrawColor(w->renderer, 214, 214, 214, 50) != 0 || SDL_RenderClear(w->renderer) != 0){
		printf("Impossible to set the background-color to this window.");
	}
	SDL_RenderPresent(w->renderer); // on actualise le renderer

	return w;
}

//ajoute une fenêtre à un liste chainée de fenêtre, retourne 0 si succès, -1 sinon
int add_window(struct window* w, char* name, int width, int height){
	if(w == NULL || name == NULL || width <= 0 || height <= 0){
		return -1;
	}
	struct window* new_w = init_window(name, width, height);
	if(new_w == NULL){
		return -1; // erreur de création de la fenêtre
	}
	if(w->pWindow == NULL){ // on ajoute une fenetre à une liste qui n'en a plus
		(*w) = (*new_w);
		return 0;
	}
	while(w->next != NULL){ // sinon on va à la fin de la liste chainée
		w = w->next;
	}
	w->next = new_w; // et on l'ajoute
	return 0;
}

int add_image(struct window* w, int key){
	if(w == NULL){ // il n'y a pas encore d'image
		return -1; // erreur
	}
	if(w->img_w == NULL){ // pas encore d'image
		w->img_w = malloc(sizeof(struct image_window));
		if(w->img_w == NULL){
			return -1; //erreur d'allocation
		}
		w->img_w->key_image = key;
		w->img_w->posx = 0; // en haut à gauche par défaut
		w->img_w->posy = 0;
		w->img_w->next = NULL;
		return 1; //succès
	}
	struct image_window* img_w = w->img_w;
	while(img_w->next != NULL){
		img_w = img_w->next;
	}
	img_w->next = malloc(sizeof(struct image_window));
	img_w->next->key_image = key;
	img_w->next->posx = 0;
	img_w->next->posy = 0;
	img_w->next->next = NULL;
	return 1; // succès
}

//Ferme une fenetre
void close_window(struct window* w){
	SDL_DestroyRenderer(w->renderer);
	SDL_DestroyWindow(w->pWindow);
}

//ferme la SDL
void end_SDL(){
	SDL_Quit();
}

// attend un evenement des fenetres et agit indéfiniment jusqu'à ce que toute les fenêtres soient fermées alors retourne -1 (pour quitter le programme) ou l'evenement [c] (touche c appuyée) -> demande de l'utilisateur pour rentrer une commande et retourne 0
int wait_event_react_until_quit_or_ask(struct window* w){
	SDL_Event event;
	while(1){
		SDL_WaitEvent(&event);
		if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_CLOSE){
				if(SDL_GetWindowFromID(event.window.windowID) == w->pWindow){
					if(w->next == NULL){ // une seule fenetre
						close_window(w);
						// on ne free(w) pas car on garde la première struct window comme tete de liste
						return -1; // plus de fenetre
					}
					close_window(w);
					(*w) = (*w->next);
				}
				else{
					struct window* tmp_w = w;
					struct window* tmp_w_n = w->next; // non null il y a au moins deux fenetres
					while(SDL_GetWindowFromID(event.window.windowID) != tmp_w_n->pWindow){
						tmp_w = tmp_w_n;
						tmp_w_n = tmp_w_n->next;
					}
					tmp_w->next = tmp_w_n->next; // on supprime la window on concervant la liste chainée
					close_window(tmp_w_n);
					free(tmp_w_n);
				}
			}
			else if(event.window.event == SDL_WINDOWEVENT_RESIZED){
				struct window* tmp_w = w;
				while(SDL_GetWindowFromID(event.window.windowID) != tmp_w->pWindow){
					tmp_w = tmp_w->next;
				}
				if(tmp_w != NULL){
					SDL_RenderClear(tmp_w->renderer); 
					if(tmp_w->img_w != NULL && tmp_w->img_w->texture != NULL && tmp_w->img_w->position_texture != NULL){
						SDL_RenderCopy(tmp_w->renderer, tmp_w->img_w->texture, NULL, tmp_w->img_w->position_texture);
					}
					SDL_RenderPresent(tmp_w->renderer);
					SDL_UpdateWindowSurface(tmp_w->pWindow);
				}
			}
		}
		else if(event.type == SDL_KEYDOWN){
			int x, y;
			if(event.key.keysym.scancode == SDL_SCANCODE_LEFT){
				SDL_GetWindowPosition(SDL_GetWindowFromID(event.window.windowID), &x, &y);
		        SDL_SetWindowPosition(SDL_GetWindowFromID(event.window.windowID), x-5, y);
			}
			else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
				SDL_GetWindowPosition(SDL_GetWindowFromID(event.window.windowID), &x, &y);
		        SDL_SetWindowPosition(SDL_GetWindowFromID(event.window.windowID), x+5, y);
			}
			else if(event.key.keysym.scancode == SDL_SCANCODE_UP){
				SDL_GetWindowPosition(SDL_GetWindowFromID(event.window.windowID), &x, &y);
		        SDL_SetWindowPosition(SDL_GetWindowFromID(event.window.windowID), x, y-5);
			}
			else if(event.key.keysym.scancode == SDL_SCANCODE_DOWN){
				SDL_GetWindowPosition(SDL_GetWindowFromID(event.window.windowID), &x, &y);
		        SDL_SetWindowPosition(SDL_GetWindowFromID(event.window.windowID), x, y+5);
			}
			else if(event.key.keysym.sym == SDLK_c){
				return 0;
			}
		}
	}
}

int load_a_image(struct window* w, char* name, char* image){
	if(w == NULL || name == NULL || image == NULL){
		return -1;
	}
	struct window* window = w;
	while(strcmp(window->name, name) != 0 && window != NULL){
		window = window->next;
	}
	if(window == NULL){ // pas de fenetre qui porte ce nom
		return -2; // erreur -2;
	}
	int i = create_img(image);
	if(i == -1){ // erreur dans la creation de l'image
		return -3; // erreur -3
	}
	if(add_image(window, i) != 1){
		return -4; //erreur d'ajout d'image
	}
	SDL_Texture* texture = malloc(sizeof(SDL_Texture*));
	struct image* img = get_img_by_key(i);
	texture = SDL_CreateTextureFromSurface(window->renderer, img->img);
	//SDL_Texture* t = IMG_LoadTexture(window->renderer, "test1.jpg");
	SDL_Rect* position_texture = malloc(sizeof(SDL_Texture*));
	position_texture->x = 0;
	position_texture->y = 0;
	int w_w, w_h;
	SDL_GetWindowSize(w->pWindow , &w_w , &w_h);
	if(img->img->h >= img->img->w){
		position_texture->w = (int)((float)img->img->w*((float)w_h/(float)img->img->h));
		position_texture->h = w_h;
	}
	else{
		position_texture->w = w_w;
		position_texture->h = (int)((float)img->img->h*((float)w_w/(float)img->img->w));
	}
	window->img_w->texture = texture;
	window->img_w->position_texture = position_texture;
	//SDL_QueryTexture(texture, NULL, NULL, &position_texture.w, &position_texture.h);
	SDL_RenderClear(window->renderer);
	SDL_RenderCopy(window->renderer, texture, NULL, position_texture);
	SDL_RenderPresent(window->renderer);
	SDL_UpdateWindowSurface(window->pWindow);
	return 1; //succes
}

//un main de test
/*int main(void){
	if(init_SDL() != 0){
		return -1;
	}
	struct window* w = init_window("Fenêtre 1", 600, 600);
	if(w == NULL){
		return -1;
	}

	if(add_window(w,"Fenetre 2", 600, 600) == -1){
		return -1;
	}
	

	if(add_window(w,"Salut", 100, 200) == -1){
		return -1;
	}

	init_images();

	printf("*************** %d\n", load_a_image(w, "Fenêtre 1", "test1.jpg"));
	
	    wait_event_react_until_quit_or_ask(w);

	close_window(w);
	end_SDL();
	return 0;
}*/