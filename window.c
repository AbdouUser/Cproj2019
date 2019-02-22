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

//initie une fenetre de nom name, et de taille width x height, return NULL en cas d'échec
struct window* init_window(char* name, int width, int height){
	if(name == NULL || width <=0 || height <= 0){
		return NULL;
	}
	struct window* w = malloc(sizeof(struct window));
	w->name = name;
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

//Ferme une fenetre
void close_window(struct window* w){
	SDL_DestroyRenderer(w->renderer);
	SDL_DestroyWindow(w->pWindow);
}

//fermer la SDL
void end_SDL(){
	SDL_Quit();
}

// attend un evenement des fenetres et agit indéfiniment jusqu'à l'evement 'fermer la fenetre' alors retourne -1 (pour traiter la liste chainée struct window) ou l'evenement [c] (touche c appuyée) -> demande de l'utilisateur pour rentrer une commande et retourne 0
int wait_event_react_until_quit_or_ask(){
	SDL_Event event;
	while(1){
		SDL_WaitEvent(&event);
		if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_CLOSE){
				SDL_DestroyRenderer(SDL_GetRenderer(SDL_GetWindowFromID(event.window.windowID)));
				SDL_DestroyWindow((SDL_GetWindowFromID(event.window.windowID)));
				return -1;
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

	struct window* v = init_window("Fenêtre 2", 600, 600);
	if(v == NULL){
		return -1;
	}
	
	    wait_event_react_until_quit_or_ask();

	close_window(w);
	end_SDL();
	return 0;
}