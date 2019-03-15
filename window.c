#include <SDL2/SDL.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "window.h"
#include "image.h"

struct image_window{
	struct image* img;
	int posx;
	int posy;
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
	w->name = name;
	w->next = NULL;
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
		printf("%s\n", w->name);
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

//un main de test
int main(void){
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
	
	    wait_event_react_until_quit_or_ask(w);

	close_window(w);
	end_SDL();
	return 0;
}