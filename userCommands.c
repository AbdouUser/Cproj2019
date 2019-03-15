#include <stdio.h>
#include "userCommands.h"

// Fonctions reconnues par le parser
// Doivent pouvoir recevoir tous ses paramètres sous forme de string aka (char*)
// pour rajouter une commande dans le parser :
//    1: - ajouter une fonction intermédiaire qui prendra les paramatres sous forme de char*, les tranformera dans le bon types puis appelera la fonction voulu
//    2: - mettre sa signature dans userCommands.h
//    3: - dans la fonction initCommands créer un pointeur de la fonction intermédiaire créée plus tot puis créer un struc fonction efin  Rajouter ce struct fonction dans le tableau res
//    4: - Pour finir il faut mettre à jour le nombre de fonction dans le fichier parser.h dans les DEFINE

int test1(char* entier) {
  printf("\nExecution of test1 with %s",entier);
  return 0;
}

int test2(char* var, char* entier) {
  printf("\nExecution of test2 with %s and %s",var,entier);
  return 0;
}

//initCommands est utilisé par le parser elle permet de lui donner les fonctions
//Fonction qui initialise un tableau de struct fonction  passé en parametre
void initCommands(fonction* res) {
  //test1 function
  int(*pointeurTest1)(char*);
  pointeurTest1 = test1;
  fonction f1 = {"test1",1,{checkInt},pointeurTest1};
  //test2 function
  int(*pointeurTest2)(char*,char*);
  pointeurTest2 = test2;
  fonction f2 = {"test2" ,2,{checkVar, checkInt},pointeurTest2};
  //exit function
  fonction exit = {"exit",0, {},NULL};
  res[0] = f1;
  res[1] = f2;
  res[2] = exit;
}
