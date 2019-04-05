#include "window.h"
#include "parser.h"
#include "userCommands.h"
#include "main.h"


int main(void){
	init_SDL();
	struct window* w = init_window("Fenêtre 1", 600, 600);
	if(w == NULL){
		exit(0);
	}
	fonction tableauDesFonctions[NB_OF_FUNCTIONS];
	initCommands(tableauDesFonctions, w);
	while(1){
  		execParser(tableauDesFonctions);  
  		wait_event_react_until_quit_or_ask(w);
  	}
  return 0;
}