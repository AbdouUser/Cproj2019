#include "userCommands.h"
#include "parser.h"
#include "window.h"

int main(void){
  if (init_SDL() == -1) return -1;
  struct window* w = init_window("CIMP",800,500);
  if (w == NULL) return -1;
  wait_event_react_until_quit_or_ask(w);
  close_window(w);
  end_SDL();
  fonction tableauDesFonctions[NB_OF_FUNCTIONS];
  initCommands(tableauDesFonctions);
  execParser(tableauDesFonctions);
  return 0;
}
