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
struct image* copy;

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

/*
  Commande GRAYSCALEIMAGE <fenetre> <key_image>
  met l'image de key key_image dans la fenetre fenetre en niveau de gris
*/
int GRAYSCALEIMAGE(char* fenetre, char* key){
  switch(image_to_grayscale(w, fenetre, atoi(key))){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("L'image de key %s a été mise en nuances de gris.\n", key); break;
  }
  return 0;
}

/*
  Commande FILLWITH <fenetre> <key_image> <red> <green> <blue>
  rempli l'image dans la fenetre avec la couleur c
*/
int FILLWITH(char* fenetre, char* key, char* r, char* g, char* b){
  SDL_Color c;
  c.r = atoi(r);
  c.g = atoi(g);
  c.b = atoi(b);
  switch(fill_with_color(w, fenetre, atoi(key), c)){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("L'image de key %s a été rempli avec la couleur r:%s g:%s b:%s .\n", key,r,g,b); break;
  }
  return 0;
}

/*
  Commande REPLACEWITH <fenetre> <key_image> <red> <green> <blue> <interval> <newRed> <newGreen> <newBLue>
  remplace la couleur red green blue avec un interval par la couleur newr newg newb
*/
int REPLACEWITH(char* fenetre, char* key, char* r, char* g, char* b, char* interval, char* newr, char* newg, char* newb){
  switch(replace_with_color(w, fenetre, atoi(key), atoi(r),atoi(g),atoi(b), atoi(interval), atoi(newr),atoi(newg),atoi(newb))){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Pas d'image de key %s.\n", key); break;
    default: printf("La couleur r:%s g:%s b:%s dans l'image de key %s a été remplacé avec la couleur r:%s g:%s b:%s dans l'intervale %s.\n",r,g,b,key,newr,newg,newb,interval); break;
  }
  return 0;
}

/*
  Commande FILLWITH <fenetre> <key_image> <red> <green> <blue>
  rempli l'image dans la fenetre avec la couleur c
*/
int CREATESELECTION(char* fenetre, char* x1, char* y1, char* x2, char* y2){
  int key = create_selection(w, fenetre, atoi(x1), atoi(y1), atoi(x2), atoi(y2));
  switch(key){
    case -1: printf("Un argument est null.\n"); break;
    case -2: printf("Pas de fenêtre du nom %s.\n", fenetre); break;
    case -3: printf("Eurreur pendant la creation de la texture\n"); break;
    default: printf("Selection créée dans la fenetre %s avec la key :%d .\n",fenetre, key); break;
  }
  return 0;
}

//copy the image with the key <key>
int COPY(char* fenetre, char* key){
  struct window* window = getWindow(w,fenetre);
  if (window == NULL){
    printf("Pas de fenêtre du nom %s.\n", fenetre);
    return -1;
  }
  struct image* temp = get_Image_By_Key_In_Window(w,atoi(key));
  if (temp == NULL){
    printf("Pas d'image de key %s.\n", key);
    return -2;
  }
  copy = temp;
  return 0;
}

//cut the image with the key <key>
int CUT(char* fenetre, char* key){
  int test = COPY(fenetre,key);
  if(test != 0){
    return test;
  }
  if(remove_image(w,fenetre,key) != 1){
    printf("Erreur pendant la coupure de l'image.\n");
    return -3;
  }
  return 0;
}

int PASTE(char* fenetre){
  if (copy == NULL){
    printf("Aucune image/selection à copier");
    return -1;
  }
  struct window* window = getWindow(w,fenetre);
  if (window == NULL){
    printf("Pas de fenêtre du nom %s.\n", fenetre);
    return -2;
  }
  struct image *temp = copy_image(window,copy);
  if (temp == NULL ) {
    printf("Erreur pendant la copie\n");
    return -3;
  }
  
  int test = add_Image_In_Window(window,temp->texture,temp->position_texture,temp->surface);
  if(test < 0){
    printf("Erreur pendant l'ajout de la copie dans la fenetre");
    return -3;
  }
  refreshWindow(window);
  return 0;
}

//TODO :
//CREATESELECTION : ne récupère as bien le pixels structure window et affichage du rendu à revoir,
//peut etre une texture directement dans le struct window que l'on recalcule en fonction des struct images
//CHANGEBORDERS
//BLACKANDWHITE
//NEGATIVE
//SAVE


//initCommands est utilisé par le parser elle permet de lui donner les fonctions
//Fonction qui initialise un tableau de struct fonction  passé en parametre
void initCommands(fonction* res, struct window* window, struct image* img) {
  w = window;
  copy = img;
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
  fonction f10 = {"FIRSTPLANIMAGE", 2, {checkName, checkInt}, pointeurFIRSTPLANIMAGE}; 
  int(*pointeurREMOVEIMAGE)(char*, char*);
  pointeurREMOVEIMAGE = REMOVEIMAGE;
  fonction f11 = {"REMOVEIMAGE", 2, {checkName, checkInt}, pointeurREMOVEIMAGE}; 
  int(*pointeurGRAYSCALEIMAGE)(char*, char*);
  pointeurGRAYSCALEIMAGE = GRAYSCALEIMAGE;
  fonction f12 = {"GRAYSCALEIMAGE", 2, {checkName, checkInt}, pointeurGRAYSCALEIMAGE};
  int(*pointeurFILLWITH)(char*, char*,char*,char*,char*);
  pointeurFILLWITH = FILLWITH;
  fonction f13 = {"FILLWITH", 5, {checkName, checkInt, checkInt, checkInt, checkInt}, pointeurFILLWITH};
  int(*pointeurREPLACEWITH)(char*, char*,char*,char*,char*,char*, char*,char*,char*);
  pointeurREPLACEWITH = REPLACEWITH;
  fonction f14 = {"REPLACEWITH", 9, {checkName, checkInt, checkInt, checkInt, checkInt, checkInt, checkInt, checkInt, checkInt}, pointeurREPLACEWITH};
  int(*pointeurCREATESELECTION)(char*,char*,char*,char*,char*);
  pointeurCREATESELECTION = CREATESELECTION;
  fonction f15 = {"CREATESELECTION", 5, {checkName, checkInt, checkInt, checkInt, checkInt}, pointeurCREATESELECTION};
  int(*pointeurCOPY)(char*, char*);
  pointeurCOPY = COPY;
  fonction f16 = {"COPY", 2, {checkName, checkInt}, pointeurCOPY};
  int(*pointeurCUT)(char*, char*);
  pointeurCUT = CUT;
  fonction f17 = {"CUT", 2, {checkName, checkInt}, pointeurCUT};
  int(*pointeurPASTE)(char*, char*);
  pointeurPASTE = PASTE;
  fonction f18 = {"PASTE", 1, {checkName}, pointeurPASTE};

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
  res[12] = f12;
  res[13] = f13;
  res[14] = f14;
  res[15] = f15;
  res[16] = f16;
  res[17] = f17;
  res[18] = f18;
}
