#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"




//  -- PARSER -- //

//PARTIE ANALYSE D'ARGUMENTS
short isAlphanumeric(char c){
  return ((c>= '0' && c<= '9') ||( c>='a' && c <+'z') ||( c>='A' && c <+'Z'));
}


short isInteger(char c){
  return (c>= '0' && c<= '9');
}


short isAlphabetic(char c){
  return ((c>='a' && c <+'z') || (c>='A' && c <+'Z'));
}

char* checkInt(char* s, int* pos){
  char* res = malloc (sizeof (char) * CHAR_LIMIT);
  int posRes = 0;
  if (s[*pos] == '-'){
    res[posRes]=s[*pos];
    *pos += 1;
    posRes += 1;
  }
  do {
    if (isInteger(s[*pos])) {
      res[posRes]=s[*pos];
      *pos += 1;
      posRes += 1;
    }
    else {
      return NULL;
    }
  }
  while (s[*pos]!=' ' && s[*pos]!='\0' &&  s[*pos]!='\n');
  res[posRes]='\0';
  return res;
}

char* checkVar(char* s, int* pos){
  char* res = malloc (sizeof (char) * CHAR_LIMIT);
  int posRes = 0;
 /* if(s[*pos]!='$'){
    return NULL;
  }*/
  //res[posRes]=s[*pos];
  //*pos += 1;
  posRes += 1;
  if (!isAlphabetic(s[*pos])){
    return NULL;
  }
  res[posRes]=s[*pos];
  *pos += 1;
  posRes += 1;
  do {
    if (isAlphanumeric(s[*pos])) {
      res[posRes]=s[*pos];
      *pos += 1;
      posRes += 1;
    }
    else { return 0;}
  }
  while (s[*pos]!=' ' && s[*pos]!='\0' &&  s[*pos]!='\n');
  res[posRes]='\0';
  return res;
}


fonction* searchFunction (char* s, int* pos, fonction* funcTab) {
  char funcRead[CHAR_LIMIT];
  int posFuncRead = 0;
  do {
    funcRead[posFuncRead] = s[*pos];
    posFuncRead += 1;
    *pos += 1;
  } while (s[*pos] != ' ' && s[*pos] != '\0' && s[*pos] != EOF && s[*pos] != '\n' );
  
  funcRead[posFuncRead] = '\0';
  for (int i = 0; i<NB_OF_FUNCTIONS ; i++){
    if (strcmp(funcRead,funcTab[i].funcName) == 0){
      return &funcTab[i];
    }
  }
  printf("No function called %s has been found\n",funcRead);
  return NULL;
}



//PARTIE TRAITEMENT

void normalise (char* s) {
//retire les espaces en trop dans la chaine de caractères de ref' s
  if (s != NULL) {
    for (int i = 0; s[i] != '\0'; i++){
      if (s[i] == ' ') {
        int dec = 0;
        int j = i+1;
        while (s[j] == ' ') {
          j++;
          dec++;
        }
        for(j=i+1; s[j] != '\0'; j++){
          s[j]=s[j+dec];
        }
      }
    }
  }
}


//PARTIE ENTREE UTILISATEUR
void userCommande (char* s){
//récupère une commande sous forme de chaine de caractères
  //AUTOCOMPLETION à rajouter ici
  fgets(s,CHAR_LIMIT, stdin);
}

//fonction qui execute le parser dans le terminal
int execParser(fonction* tableauDesFonctions) {
  int lastFuncCalledValue = 0;
  /*
  fonction tableauDesFonctions[NB_OF_FUNCTIONS];
  initCommands(&tableauDesFonctions);
  */
  //Execution du parser
  do {
    printf("\n::>");
    //attente d'une entrée utilisateur
    char input[CHAR_LIMIT];
    int pos = 0;
    fgets(input,CHAR_LIMIT, stdin);
    //userCommande(input);
    //traitement de l'entrée
    normalise(input);
    //analyse de l'entrée
    fonction* f = searchFunction(input, &pos, tableauDesFonctions);
    short noErrorOccurred=1;
    //COMMANDE INCONNUE
    if (f == NULL) {
      continue;
    }
    /*COMMANDE window qui permet à l'utilisateur d'être en mode fenêtre 
    et pas en mode invite de commande*/
    else if (strcmp(f->funcName,"window") == 0) {
      if(input[pos] == ' '){
        pos += 1;
        if(input[pos] != '\0' && input[pos] != EOF && input[pos] != '\n' ){
          char* entier = checkInt(input, &pos);
          if (entier == NULL){
            printf("\nParamètre non reconnu.");
            continue;
          }
          char* ptrEnd;
          long res = strtol(entier, &ptrEnd, 10);
          free(entier);
          return res;
        }
        else {
          return lastFuncCalledValue;
        }
      }
      else if (input[pos] == '\0' || input[pos] == EOF || input[pos] == '\n' ){
        return lastFuncCalledValue;
      }
      else {
        printf("\nCaractère \'%c\'non attendu position %d.\n",input[pos],pos);
        continue;
      }
    }
    else if(strcmp(f->funcName,"exit") == 0){ // commande pour quitter le programme
      exit(0);
    }
    //COMMANDE RECONNUE
    else {
      char* paramTab[f->nb_param];
      //anaylse des paramètres attendus
      for (int i = 0; i < f->nb_param; i++) {
  pos+=1;//l'espace avant chaque paramètre
  char* testParam = f->listeParam[i](input, &pos);
  if (testParam == NULL) {
    printf("\nLe paramètre %d est inccorect\n",i+1);
    noErrorOccurred = 0;
    break;
  }
  else {
    paramTab[i] = testParam;//add the string to the paramtab
  }
}
if (noErrorOccurred == 1) {
      //Check if end of string reached
  if (input[pos] == ' ') {pos++;}//espace eventuel
  if (input[pos] != '\0' && input[pos] != '\n' && input[pos] != EOF) {
    noErrorOccurred = 0;
    printf("\nCaractère \'%c\'non attendu position %d.\n",input[pos],pos);
  }//End of string
}
if (noErrorOccurred == 1){
  //si correcte executer la commande respective :
  switch(f->nb_param){
    case 0 :
    lastFuncCalledValue = (f->pointeurFunction)();
    break;
    case 1 :
    lastFuncCalledValue = f->pointeurFunction(paramTab[0]);
    free(paramTab[0]);
    break;
    case 2 :
    lastFuncCalledValue = (*f->pointeurFunction)(paramTab[0],paramTab[1]);
    free(paramTab[0]);
    free(paramTab[1]);
    break;
    case 3 :
    lastFuncCalledValue = (*f->pointeurFunction)(paramTab[0],paramTab[1],paramTab[2]);
    free(paramTab[0]);
    free(paramTab[1]);
    free(paramTab[2]);
    break;
    case 4 :
    lastFuncCalledValue = (*f->pointeurFunction)(paramTab[0],paramTab[1],paramTab[2],paramTab[3]);
    free(paramTab[0]);
    free(paramTab[1]);
    free(paramTab[2]);
    free(paramTab[3]);
    break;
    case 5 :
    (*f->pointeurFunction)(paramTab[0],paramTab[1],paramTab[2],paramTab[3],paramTab[4]);
    free(paramTab[0]);
    free(paramTab[1]);
    free(paramTab[2]);
    free(paramTab[3]);
    free(paramTab[4]);
    break;
    default:
    printf("\nNombre de paramètres incorrect\n");
    break;
  }
}
}
  }while(1); //boucle infini
}

//Reste à faire :
//-Ajout des pointeurs des fonctions pour leurs execution dynamique