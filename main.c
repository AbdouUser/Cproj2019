#include "window.h"
#include "parser.h"
#include "userCommands.h"


int main(void){
	init_SDL(); // on initie la SDL

		// on ouvre la première fenêtre lors du lancement du programme
	struct window* w = init_window("CIMP", 600, 300);
	if(w == NULL){ // on quitte si erreur.
		printf("Erreur dans la création de la première fenêtre.\n");
		exit(0);
	}

	fonction tableauDesFonctions[NB_OF_FUNCTIONS];
	initCommands(tableauDesFonctions, w);

	//For testing purpose :
	int papKey = load_An_Image(w,"CIMP","papillon.jpg");
	move_image(w,"CIMP",papKey,50,50);
	struct image* img = get_Image_By_Key_In_Window(w,papKey);

	

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