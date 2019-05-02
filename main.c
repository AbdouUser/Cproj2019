#include "window.h"
#include "parser.h"
#include "userCommands.h"


int main(void){
	init_SDL(); // on initie la SDL

		// on ouvre la première fenêtre lors du lancement du programme
	struct window* w = init_window("CIMP", 800, 500);
	if(w == NULL){ // on quitte si erreur.
		printf("Erreur dans la création de la première fenêtre.\n");
		exit(0);
	}

	fonction tableauDesFonctions[NB_OF_FUNCTIONS];
	struct image* copy = NULL;
	initCommands(tableauDesFonctions, w, copy);
	
	//initialisation d'une memoire pour la copie d'image/selection

	//TEST
	/*
	int k = load_An_Image(w,"CIMP","papillon.jpg");
	//move_image(w,"CIMP",k, 10,10);
	int key = create_selection(w,"CIMP",50,50,100,100);
	SDL_Color c;
	c.a = 100;
	c.r = 0;
	c.g = 0;
	c.b = 100;
	fill_with_color(w,"CIMP",key,c);
	move_image(w,"CIMP",key,50,50);
	//refreshWindow(w);
	*/


	// boucle principale
	while(1){
		// on demande une commande à l'utilisateur
		execParser(tableauDesFonctions);  

		/* on attend une réaction des fenêtres 
		(potentiellement appui de la touche 'c' qui quitte la fonction 
		et donc rend de nouveau l'invite de commande actif.)*/
		wait_event_react_until_quit_or_ask(w);
	}
	return 0;
}