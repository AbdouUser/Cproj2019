#include <stdio.h>
#include "userCommands.h"
#include "window.h"

// Fonctions reconnues par le parser
// Doivent pouvoir recevoir tous ses paramètres sous forme de string aka (char*)
// pour rajouter une commande dans le parser :
//    1: - ajouter une fonction intermédiaire qui prendra les paramatres sous forme de char*, les tranformera dans le bon types puis appelera la fonction voulu
//    2: - mettre sa signature dans userCommands.h
//    3: - dans la fonction initCommands créer un pointeur de la fonction intermédiaire créée plus tot puis créer un struc fonction efin  Rajouter ce struct fonction dans le tableau res
//    4: - Pour finir il faut mettre à jour le nombre de fonction dans le fichier parser.h dans les DEFINE

struct window* w;

int test1(char* entier) {
  printf("\nExecution of test1 with %s",entier);
  return 0;
}

int test2(char* var, char* entier) {
  printf("\nExecution of test2 with %s and %s",var,entier);
  return 0;
}
/*
  Commande pour créer une fenêtre
  Syntaxe : NEWWINDOW <name> <width> <height>
*/
int NEWWINDOW(char* var, char* entier1, char* entier2){
  add_window(w, var, atoi(entier1), atoi(entier2));
  return 0;
}

int LOADIMAGE(char* image, char* fenetre){
  int err = load_An_Image(w, fenetre, image);
  switch (err)
  {
  case -1:
    printf("Un argument est est null.\n");
    break;
  case -2:
    printf("Pas de fenetre du nom %s.\n",fenetre);
    break;
  case -3:
    printf("Erreur dans la creation de l'image.\n");
    break;
  case -4:
    printf("Erreur dans l'ajout de l'image dans la fenetre.\n");
    break;
  default:
    printf("L'image %s a bien été chargée dans la fenêtre %s.\n",image, fenetre);
    break;
  }
  return 0;
}

int MOVEIMAGE(char* fenetre, char* key, char* posx, char* posy){
  int err = move_image(w,fenetre,atoi(key),atoi(posx),atoi(posy));
  switch (err)
  {
  case -1:
    printf("Un argument est est null.\n");
    break;
  case -2:
    printf("Pas de fenetre du nom %s.\n",fenetre);
    break;
  case -3:
    printf("Pas d'image de key %s.\n",key);
    break;
  default:
    printf("L'image de key %s a bien été déplacée dans la fenêtre %s.\n",key, fenetre);
    break;
  }
  return 0;
}

//with the -1 key it will modifie all the images and selections in the window
//the -1 key doesn't work yet
int GREYLEVELS(char* fenetre, char* key) {
  struct window* window = getWindow(w,fenetre);
  if (window == NULL) {
    printf("Pas de fenetre du nom %s.\n",fenetre);
    return 0;
  }
  SDL_Rect* rectangle = malloc(sizeof(SDL_Rect));
  SDL_Texture* texture;
  if(atoi(key) == -1) {
	  struct image* img = window->img_w;
    while(img != NULL) {
      rectangle = img->position_texture;
      texture = img->texture;
      int t = greyLevels(window->renderer,rectangle,texture);
      if (t != 0 ){
        printf("Erreur pendant le traitement d'une des images.");
        return 0;
      }
      if(img->next != NULL){
        img = img->next;
      }
      else{
        break;
      }
    }
  }
  else {
	  struct image* img = get_Image_By_Key_In_Window(window, atoi(key));
    if (img == NULL){
      printf("Pas d'image de key %s.\n",key);
      return 0;
    }
    rectangle = img->position_texture;
    texture = img->texture;
    int t = greyLevels(window->renderer,rectangle,texture);
    if (t != 0 ){
      printf("Erreur pendant le traitement de l'image.");
      return 0;
    }
  }
	refreshWindow(window);
  return 1;
}

//TODO :
//CREATESELECTION
//RESIZE
//CHANGEBORDERS
//FILL
//REPLACE
//BLACKANDWHITE
//NEGATIVE
//COPY
//CUT
//PASTE
//SAVE


//initCommands est utilisé par le parser elle permet de lui donner les fonctions
//Fonction qui initialise un tableau de struct fonction  passé en parametre
void initCommands(fonction* res, struct window* window) {
  w = window;
  //test1 function
  int(*pointeurTest1)(char*);
  pointeurTest1 = test1;
  fonction f1 = {"test1",1,{checkInt},pointeurTest1};
  //test2 function
  int(*pointeurTest2)(char*,char*);
  pointeurTest2 = test2;
  fonction f2 = {"test2" ,2,{checkVar, checkInt},pointeurTest2};
  int(*pointeurNEWWINDOW)(char*, char*, char*);
  pointeurNEWWINDOW = NEWWINDOW;
  fonction f4 = {"NEWWINDOW", 3, {checkName, checkInt, checkInt}, pointeurNEWWINDOW};
  int(*pointeurLOADIMAGE)(char*, char*);
  pointeurLOADIMAGE = LOADIMAGE;
  fonction f5 = {"LOADIMAGE", 2, {checkName, checkName}, pointeurLOADIMAGE};
  int(*pointeurMOVEIMAGE)(char*, char*, char*, char*);
  pointeurMOVEIMAGE = MOVEIMAGE;
  fonction f6 = {"MOVEIMAGE", 4, {checkName, checkInt, checkInt, checkInt}, pointeurMOVEIMAGE};
  int(*pointeurGREYLEVELS)(char*, char*);
  pointeurGREYLEVELS = GREYLEVELS;
  fonction f7 = {"GREYLEVELS", 2, {checkName, checkInt}, pointeurGREYLEVELS};
  //exit function
  fonction wind = {"window",0, {},NULL};
  fonction exit = {"exit", 0, {}, NULL};
  res[0] = f1;
  res[1] = f2;
  res[2] = exit;
  res[3] = f4;
  res[4] = wind;
  res[5] = f5;
  res[6] = f6;
  res[7] = f7;
}
