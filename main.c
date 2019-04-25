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
	initCommands(tableauDesFonctions, w);

	//For testing purpose :
	/*
	int key = load_An_Image(w,"CIMP","papillon.jpg");
	key = load_An_Image(w,"CIMP","test80.png");
	key = load_An_Image(w,"CIMP","test80.png");
	move_image(w,"CIMP",key,80,0);
	key = load_An_Image(w,"CIMP","test80.png");
	move_image(w,"CIMP",key,160,0);
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