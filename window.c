#include <SDL2/SDL.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "image.h"

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
	// a ce moment w->name n'est pas une chaine vide mais contient des caractères exotiques
	w->name = name;
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

struct window* getWindow (struct window* w, char* windowName) {
	if (w == NULL){
		return NULL;
	}
	struct window* window = w;
	while(window != NULL && strcmp(window->name, windowName) != 0){
		window = window->next;
	}
	return window;
}

//return the key of the image which has been added
int add_Image_In_Window(struct window* w, SDL_Texture* texture, SDL_Rect* position_texture, SDL_Surface* surface){
	if(w == NULL){ // il n'y a pas de fenetre
		return -1; // erreur
	}
	struct image* img = add_New_Image(w->img_w,texture,position_texture, surface);
	if (w->img_w == NULL){
		w->img_w = img;
	}
	printf("Image ajouté avec la key : %d\n",img->key_image);
	return img->key_image; 
}

struct image *get_Image_By_Key_In_Window(struct window* w, int key){
	if(w == NULL){ // il n'y a pas de fenetre
		return NULL;
	}
	return get_Image_By_Key(w->img_w,key); 
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
					/*
					SDL_RenderClear(tmp_w->renderer); 
					if(tmp_w->img_w != NULL && tmp_w->img_w->texture != NULL && tmp_w->img_w->position_texture != NULL){
						SDL_RenderCopy(tmp_w->renderer, tmp_w->img_w->texture, NULL, tmp_w->img_w->position_texture);
					}
					SDL_RenderPresent(tmp_w->renderer);
					SDL_UpdateWindowSurface(tmp_w->pWindow);
					*/
					refreshWindow(tmp_w);
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

//Refresh a window / clear the window and draw all the image.
void refreshWindow(struct window* w){
	SDL_RenderClear(w->renderer);
	struct image* img = w->img_w;
	while (img != NULL) {
		if (SDL_RenderCopy(w->renderer, img->texture, NULL, img->position_texture) != 0){
			SDL_GetError();
		}
		img = img->next;
	}
	SDL_RenderPresent(w->renderer);
	SDL_GetWindowSurface(w->pWindow);
	if (SDL_UpdateWindowSurface(w->pWindow) != 0){
		SDL_GetError();
	}
	
}

int load_An_Image(struct window* w, char* name, char* image){
	if(w == NULL || name == NULL || image == NULL){
		return -1;
	}
	struct window* window = getWindow(w,name);
	if(window == NULL){ // pas de fenetre qui porte ce nom
		return -2; // erreur -2;
	}
	
	SDL_Surface *img=NULL; // on crée une surface pour charger l'image
	img=IMG_Load(image); // on charge l'image
	if(img == NULL){ // erreur dans la creation de l'image
		return -3; // erreur -3
	}
	int x = img->w; // on récupère les dimensions de l'image
	int y = img->h;
	SDL_Texture* texture_tmp; // on crée une texture tampon dans laquelle on va charger l'image
	SDL_Texture* texture = malloc(sizeof(SDL_Texture*)); // texture officielle de l'image
	texture_tmp = SDL_CreateTextureFromSurface(window->renderer, img); // on charge l'image dans la texture tampon : c'est texture n'a pas les droits nécessaires pour retoucher l'image
	if(texture_tmp == NULL){ // erreur
		return -3;
	}
	// on crée la texture officielle avec les droits de retouches
	texture = SDL_CreateTexture(window->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, x, y);
	if(texture == NULL){
		return -3;
	}
	//on copie la texture tampon dans la texture officielle
	SDL_SetRenderTarget(window->renderer, texture);
	SDL_RenderCopy(window->renderer, texture_tmp, NULL, NULL);
	SDL_DestroyTexture(texture_tmp);
	//SDL_FreeSurface(img);
	SDL_SetRenderTarget(window->renderer, NULL);

	SDL_Rect* position_texture = malloc(sizeof(SDL_Texture*));
	position_texture->x = 0;
	position_texture->y = 0;
	position_texture->w = x;
	position_texture->h = y;
	/*
	int w_w, w_h;
	SDL_GetWindowSize(w->pWindow , &w_w , &w_h);
	if(img->h >= img->w){
		position_texture->w = (int)((float)img->w*((float)w_h/(float)img->h));
		position_texture->h = w_h;
	}
	else{
		position_texture->w = w_w;
		position_texture->h = (int)((float)img->h*((float)w_w/(float)img->w));
	}
	*/
	//SDL_QueryTexture(texture, NULL, NULL, &position_texture->w, &position_texture->h);
	int key = add_Image_In_Window(window, texture, position_texture, img);
	if(key < 0){
		return -4; //erreur d'ajout d'image
	}
	refreshWindow(window);
	return key; //success
}


/*
	Fonction qui déplace une image dans sa fenêtre :
		w : le début de la liste chainée de fenetre
		name : le nom de la fenêtre où se trouve l'image à déplacer
		img_key : le nom de l'image
		x_pixels : le nombre de pixel qu'on déplace sur x
		y_pixels : le nombre de pixel qu'on déplace sur y
*/
int move_image(struct window* w, char* name, int img_key, int x_pixels, int y_pixels){
	if(w == NULL || name == NULL){
		return -1;
	}
	struct window* window = getWindow(w,name);
	if(window == NULL){ // pas de fenetre qui porte ce nom
		return -2; // erreur -2;
	}
	struct image* image = get_Image_By_Key_In_Window(window, img_key);
	if(image == NULL){ // pas d'image avec cette key
		return -3;
	}
	image->position_texture->x += x_pixels;
	image->position_texture->y += y_pixels;
	refreshWindow(window);
	return 1;
}

int resize_image(struct window* w, char* name_w, int img_key, int width, int height){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	// on récupère l'image
	struct image* image = get_Image_By_Key_In_Window(window, img_key);
	if(image == NULL){ // pas d'image avec cette key
		return -3;
	}
	set_size(image, width, height);
	refreshWindow(window);
	return 1;
}

//applique un facteur de zoom sur l'image de key img_key dans la fenetre name_w
int zoom_image(struct window* w, char* name_w, int img_key, double zoom){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	struct image* image = get_Image_By_Key_In_Window(window, img_key);
	if(image == NULL){ // pas d'image avec cette key
		return -3;
	}
	zoom_on_a_image(image, zoom);
	refreshWindow(window);
	printf("%s %d : %d x %d\n", name_w, img_key, image->position_texture->w, image->position_texture->h);
	return 1;
}

//effectue un zoom sur toute une window
int zoom_window(struct window* w, char* name_w, double zoom){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	if(window->img_w == NULL){ // aucune image
		return 1;
	}
	struct image* image = window->img_w;
	while(image != NULL){
		zoom_on_a_image(image, zoom);
		image = image->next;
	}
	refreshWindow(window);
	return 1;
}

int remove_image(struct window* w, char* name_w, int img_key){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	if(window->img_w == NULL){
		return -3;
	}
	struct image* image = window->img_w;
	if(image->key_image == img_key){ // c'est la première image
		window->img_w = image->next;
		free(image);
		refreshWindow(window);
		return 1;
	}
	while(image->next != NULL && image->next->key_image != img_key){
		image = image->next;
	}
	if(image->next == NULL){
		return -3;
	}
	struct image* i_suppr = image->next; // on la stocke pour pouvoir la free
	image->next = image->next->next;
	free(i_suppr);
	refreshWindow(window);
	return 1;
}

int image_to_first_plan(struct window* w, char* name_w, int img_key){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	if(window->img_w == NULL){
		return -3;
	}
	if(window->img_w->next == NULL){ // une seule image
		return 1;
	}

	struct image* test = window->img_w;
	while(test != NULL){
		test = test->next;
	}

	struct image* image = window->img_w;
	struct image* image_s;
	if(image->key_image == img_key){ 
		image_s = image;
		window->img_w = image->next;
	}
	else{
		while(image->next != NULL && image->next->key_image != img_key){
			image = image->next;
		}
		if(image->next == NULL){
			return -3;
		}
		image_s = image->next;
		image->next = image->next->next;
	}
	image_s->next = NULL; // c'est la dernière image
	image = window->img_w;
	while(image->next != NULL){ // on va à la dernière image
		image = image->next;
	}
	image->next = image_s; // on ajoute notre image
	test = window->img_w;
	while(test != NULL){
		test = test->next;
	}
	refreshWindow(window);
	return 1;
}

int image_to_grayscale(struct window* w, char* name_w, int img_key){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	struct image* image = get_Image_By_Key_In_Window(window, img_key);
	if(image == NULL){ // pas d'image avec cette key
		return -3;
	}

	// on crée un format RGBA sur 8bit
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	//on convertit la surface en RGBA sur 8bit
	image->surface = SDL_ConvertSurface(image->surface, format ,0);

	//on la lock pour pouvoir la modifier
	SDL_LockSurface(image->surface);

	// on récupère ses pixels
	Uint32* pixels = image->surface->pixels;
	size_t i, j;
	for(i = 0; i < image->surface->h; i++){
   	 for(j = 0; j < image->surface->w; j++){
   	 	SDL_Color c;
   	 	// on recupère la couleur du pixel
   	 	SDL_GetRGBA(pixels[i * image->surface->w + j], image->surface->format, &c.r, &c.g, &c.b, &c.a);
   	 	// calcule de la nuance de gris selon le standart Rec. 601 pour écran numérique.
   	 	int gray = (int)(0.299*(double)c.r + 0.587*(double)c.g + 0.114*(double)c.b);
        pixels[i * image->surface->w + j] = SDL_MapRGBA(image->surface->format, (Uint8)gray, (Uint8)gray, (Uint8)gray, c.a);
		}
	}

	image->texture = SDL_CreateTextureFromSurface(window->renderer, image->surface);

	refreshWindow(window);

	return 1;

}

int fill_with_color(struct window* w, char* name_w, int img_key, SDL_Color c){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	struct image* image = get_Image_By_Key_In_Window(window, img_key);
	if(image == NULL){ // pas d'image avec cette key
		return -3;
	}

	// on crée un format RGBA sur 8bit
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	//on convertit la surface en RGBA sur 8bit
	image->surface = SDL_ConvertSurface(image->surface, format ,0);

	//on la lock pour pouvoir la modifier
	SDL_LockSurface(image->surface);

	// on récupère ses pixels
	Uint32* pixels = image->surface->pixels;
	size_t i, j;
	for(i = 0; i < image->surface->h; i++){
   	 for(j = 0; j < image->surface->w; j++){
        pixels[i * image->surface->w + j] = SDL_MapRGBA(image->surface->format, c.r, c.g, c.b, c.a);
		}
	}

	image->texture = SDL_CreateTextureFromSurface(window->renderer, image->surface);

	refreshWindow(window);

	return 1;
}

int replace_with_color(struct window* w, char* name_w, int img_key, Uint8 r, Uint8 g, Uint8 b,Uint8 interval, Uint8 newr, Uint8 newg, Uint8 newb){
	if(w == NULL || name_w == NULL){
		return -1;
	}
	struct window* window = getWindow(w, name_w); // on recupère la bonne fenetre
	if(window == NULL){ // pas de fenêtre qui porte ce nom
		return -2;
	}
	struct image* image = get_Image_By_Key_In_Window(window, img_key);
	if(image == NULL){ // pas d'image avec cette key
		return -3;
	}

	// on crée un format RGBA sur 8bit
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	//on convertit la surface en RGBA sur 8bit
	image->surface = SDL_ConvertSurface(image->surface, format ,0);

	//on la lock pour pouvoir la modifier
	SDL_LockSurface(image->surface);

	// on récupère ses pixels
	Uint32* pixels = image->surface->pixels;
	size_t i, j;
	for(i = 0; i < image->surface->h; i++){
   		for(j = 0; j < image->surface->w; j++){
			SDL_Color c;
			// on recupère la couleur du pixel
			SDL_GetRGBA(pixels[i * image->surface->w + j], image->surface->format, &c.r, &c.g, &c.b, &c.a);
			if(c.r >= r-interval && c.r <= r+interval &&
				c.g >= g-interval && c.g <= g+interval &&
				c.b >= b-interval && c.b <= b+interval){
				pixels[i * image->surface->w + j] = SDL_MapRGBA(image->surface->format, newr, newg, newb, c.a);
			}
		}
	}

	image->texture = SDL_CreateTextureFromSurface(window->renderer, image->surface);

	refreshWindow(window);

	return 1;
}