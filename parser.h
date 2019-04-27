#ifndef PARSER_H
#define PARSER_H

#define CHAR_LIMIT 500 //longueur max en char des commandes de l'utilisateur dans le shell
#define NB_OF_FUNCTIONS 15 //Nombre de commandes totales !!ATTENTION A BIEN VERIFIER!!
#define NB_PARAM_MAX 10 //Ne pas modifier sans modifier la partie du main avec le switch statement

//une fonction est une struct qui contient les informations necessaire au fonctionnement du parser
typedef struct fonction {
  //la chaine de caractère correpondant au nom de la commande écrite dans le terminal
  char* funcName;
  //tableau de fonctions traitant chaque paramètre : par exemple pour une fonction ayant deux entiers en paramètre -> {checkInt, checkInt}
  int nb_param;
  char* (*listeParam[NB_PARAM_MAX])(char*,int*);
  //pointeur vers sa fonction
  int (*pointeurFunction)();
}fonction;
//ces fonctions sont dans un tableau

short isAlphanumeric(char c);
short isInteger(char c);
char* checkName(char* s, int* pos);
short isAlphabetic(char c);
char* checkInt(char* s, int* pos);
char* checkDouble(char* s, int* pos);
char* checkVar(char* s, int* pos);
fonction* searchFunction (char* s, int* pos, fonction* funcTab);
void normalise (char* s);
void userCommande (char* s);
int execParser(fonction* tf);


#endif
