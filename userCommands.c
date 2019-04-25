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

/*
* Commande LOADIMAGE <image> <fenetre> 
* ou image est le chemin de l'image
*/

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

/*
* Commande MOVEIMAGE <fenetre> <key_image> <x> <y>
*/

int MOVEIMAGE(char* fenetre, char* key, char* posx, char* posy){
  int err = move_image(w,fenetre,atoi(key),atoi(posx),atoi(posy));
  switch (err)
  {
  case -1:
    printf("Un argument est est null.\n");
    break;
  case -2:
    printf("Pas de fenêtre du nom %s.\n",fenetre);
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

/*
 * Commande RESIZEIMAGE <fenetre> <key_image> <new_width> <new_height>
*/

int RESIZEIMAGE(char* fenetre, char* key, char* width, char* height){

  switch(resize_image(w, fenetre, atoi(key), atoi(width), atoi(height))){

    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("L'image de key %s a bien été redimmensionnée dans la fenêtre %s.\n", key, fenetre); break;
  }
  return 0;

}
/*  
* Commande ZOOMIMAGE <fenetre> <key_image> <zoom>
* zoom est un facteur de zoom 0.1, 2 etc..
*/
int ZOOMIMAGE(char* fenetre, char* key, char* zoom){
  switch(zoom_image(w, fenetre, atoi(key), atof(zoom))){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("Le zoom sur l'image de key %s a bien été effectué dans la fenêtre %s.\n", key, fenetre); break;
  }
  return 0;
}

/*  
* Commande ZOOMIMAGE <fenetre> <zoom>
* zoom est un facteur de zoom 0.1, 2 etc..
*/
int ZOOMWINDOW(char* fenetre, char* zoom){
  switch(zoom_window(w, fenetre, atof(zoom))){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    default: printf("Le zoom a bien été effectué dans la fenêtre %s.\n", fenetre); break;
  }
  return 0;
}

/*
* Commande REMOVEIMAGE <fenetre> <key_image>
* supprime l'image
*/
int REMOVEIMAGE(char* fenetre, char* key){
  switch(remove_image(w, fenetre, atoi(key))){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("L'image de key %s a bien été supprimée dans la fenêtre %s.\n", key, fenetre); break;
  }
  return 0;
}

/*
* Commande FIRSTPLANIMAGE <fenetre> <key_image>
* met l'image de key key_image en premier plan
*/
int FIRSTPLANIMAGE(char* fenetre, char* key){
  switch(image_to_first_plan(w, fenetre, atoi(key))){
     case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("L'image de key %s a bien été mise au premier plan dans la fenêtre %s.\n", key, fenetre); break;
  }
  return 0;
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
  int(*pointeurRESIZEIMAGE)(char*, char*, char*, char*);
  pointeurRESIZEIMAGE = RESIZEIMAGE;
  fonction f7 = {"RESIZEIMAGE", 4, {checkName, checkInt, checkInt, checkInt}, pointeurRESIZEIMAGE};
  int(*pointeurZOOMIMAGE)(char*, char*, char*);
  pointeurZOOMIMAGE = ZOOMIMAGE;
  fonction f8 = {"ZOOMIMAGE", 3, {checkName, checkInt, checkDouble}, pointeurZOOMIMAGE};
  int(*pointeurZOOMWINDOW)(char*, char*);
  pointeurZOOMWINDOW = ZOOMWINDOW;
  fonction f9 = {"ZOOMWINDOW", 2, {checkName, checkDouble}, pointeurZOOMWINDOW}; 
  int(*pointeurFIRSTPLANIMAGE)(char*, char*);
  pointeurFIRSTPLANIMAGE = FIRSTPLANIMAGE;
  fonction f10 = {"FIRSTPLANIMAGE", 2, {checkName, checkName}, pointeurFIRSTPLANIMAGE}; 
  int(*pointeurREMOVEIMAGE)(char*, char*);
  pointeurREMOVEIMAGE = REMOVEIMAGE;
  fonction f11 = {"REMOVEIMAGE", 2, {checkName, checkName}, pointeurREMOVEIMAGE}; 


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
  res[8] = f8;
  res[9] = f9;
  res[10] = f10;
  res[11] = f11;
}
