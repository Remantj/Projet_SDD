#include "LC.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialisation d'une liste vide
List *initList() {
  // Alloc

  List *l = (List *)malloc(sizeof(List)); // on alloue de la mémoire pour un pointeur de liste
  if (l == NULL)
    return NULL; // si l'alloc echoue en renvoie null
  *l = NULL; // met la valeur de ce pointeur à Null
  return l; // on retourne le pointeur de la liste
}

// Création d'une cellule
Cell *buildCell(char *ch) {
  // Alloc

  Cell *c = (Cell *)malloc(sizeof(Cell)); // on alloue de la mémoire pour une cellule
  if (c == NULL)
    return NULL; // si l'alloc echoue on renvoie null
  c->data = strdup(ch); // on dupplique le contenu de ch dans data
  c->next = NULL; // on met le pointeur du suivant vers null
  return c; // on retourne la Cell
}

// Libération d'une cellule
void libererCell(Cell *c) {
  free(c->data); // on libère le champ data
  free(c); // puis on libère la Cell
}

// Libération d'une liste
void libererList(List *l) {
  Cell *xs = *l;
  Cell *tmp;
  while (xs) {
    tmp = xs;
    xs = xs->next;
    libererCell(tmp);
  }
  free(l);
}

// Insertion d'une cellule en tête d'une liste, ne duplique pas la cellule
void insertFirst(List *L, Cell *C) {
  C->next = *L; // on met le pointeur de suivant la Cell vers la tête de la liste
  *L = C; // puis on met à jour le pointeur de la tête de la liste
}

// Transforme une cellule en chaîne de caractères
char *ctos(Cell *c) {
  // Alloc

  if (c == NULL) // si la cellule est null 
    return NULL; // on retourne null
  char *ch = strdup(c->data); // sinon on dupplique le contenu du champ data dans la chaîne
  return ch; // et on retourne la chaîne
}

// Transforme un liste en chaîne de caractères
// C'est à dire l'ensemble des Cell qu'il contient séparé par des "|"
char *ltos(List *L) {
  // Alloc

  if (*L == NULL) // on verifie que la liste n'est pas null
    return NULL; // Si elle est null on retourne null
  Cell *tmp = *L; // on créer une Cell qui pointe vers la tête de la liste

  // Calcul précis de la zone mémoire à réserver
  int taille = 0;
  while (tmp) { // on parcours la liste
    if (tmp->data != NULL) { // si le champ data de la Cell n'est pas null
      taille = taille + strlen(tmp->data) + 1; // on ajoute la longueur de data à taille
    }
    tmp = tmp->next; // on passe au suivant
  }
  taille++;

  tmp = *L; // on remet à jour le pointeur à la tête de la liste
  if (tmp->data == NULL)
    return NULL; // si la première Cell a un champ data Null on retourne Null
  char *ch = (char *)malloc(taille * sizeof(char)); // sinon on alloue une chaine de caractère d'une certaine taille calculé auparavant
  strcpy(ch, tmp->data); // on copie la première Cell dans la chaîne de caractère
  tmp = tmp->next;
  while (tmp) { // on parcours la liste
    strcat(ch, "|"); // on ajoute le caractère "|" permettant de séparer les éléments
    strcat(ch, (tmp)->data); // on ajoute l'élément dans la chaîne de caractère
    tmp = tmp->next; // on passe à l'éléments suivant
  }
  return ch; // on finit par retourner la chaîne de caractère
}

// Retourne le i-ème élément de la liste, ne fait pas de duplication
// On suppose que i < len(L)
Cell *listGet(List *L, int i) {
  Cell *tmp = *L; // on créer une Cell qui pointe vers la tête de la liste
  while (i > 1) { // on fait une boucle qui permet d'accéder au ième éléments de la liste
    if (tmp == NULL) // si tmp est Null cela signifie qu'on a dépassé la taille de la liste
      return NULL; // donc on retourne Null
    tmp = tmp->next; // sinon on passe à l'élément suivant
    i--; // on décremente le compteur
  }
  return tmp; // on sort de la boucle, donc on retourne cet élément
}

// Cherche l'élément dont le champs name est égal à str dans la liste
// Ne duplique pas la cellule
Cell *searchList(List *L, char *str) {
  Cell *tmp = *L; // on créer une Cell qui pointe vers la tête de la liste
  if (tmp == NULL)
    return NULL; // si le pointeur est Null on retourne Null
  while (tmp) { // on parcours la liste
    if (strcmp(str, tmp->data) == 0) { // on compare à chaque fois le champ data avec str
      return tmp; // si c'est identique alors on le retourne
    }
    tmp = tmp->next; // sinon on passe au suivant
  }
  return NULL; //si on sort de la boucle , c'est qu'on ne l'a pas trouvé, donc on retourne Null
}

// Transforme une chaîne de caractères représentant un list en une liste
List *stol(char *s) {
  // Alloc
  
  List *L = initList(); // on initialise la liste
  int i_s = 0; // indice pour parcourir la chaîne s
  int i_tmp = 0; // indice pour parcourir la chaine tmp
  char *tmp = (char *)malloc(256 * sizeof(char)); // on alloue de la mémoire pour une chaîne de caractère
  Cell *c;

  // On parcourt la chaîne s
  while (s[i_s] != '\0') { // tant qu'on ne rencontre pas le caractère fin de chaîne
    if ((s[i_s] == '|') && (i_tmp > 0)) { // on regarde si le caractère est égal à "|"
      tmp[i_tmp] = '\0'; // si c'est la cas, on rajoute le caractère fin de chaîne à la chaîne de caractère tmp
      c = buildCell(tmp); // puis on créer une Cell ayant pour data tmp
      insertFirst(L, c); // on insère cette Cell dans la liste
      i_tmp = 0; // et on remet à 0, l'indice pour la chaîne tmp
    } else {
      tmp[i_tmp] = s[i_s]; // sinon on copie caractère par caractère 
      i_tmp++; // on incrémente l'indice de parcours pour tmp
    }
    i_s++; // on incrément l'indice de parcours pour s
  }

  if (i_tmp > 0) { // on regarde si l'indice tmp est supérieur à 0
    tmp[i_tmp] = '\0'; // si c'est le cas, cela signifie qu'il reste une dernière Cell qu'on a pas encore ajouté
    c = buildCell(tmp); // donc on ajoute cette dernière Cell à la liste
    insertFirst(L, c);
  }
  free(tmp);
  return L; // on retourne la Liste
}

// Ecris la représentation sous forme de chaîne de caractère d'une liste dans un fichier
void ltof(List *L, char *path) {
  FILE *f = fopen(path, "w"); // on essaie d'ouvrir le fichier en mode écriture
  if (f == NULL){
    printf("Erreur lors de l'ouverture du fichier %s\n", path); // si ça echoue on envoie un message d'erreur
    return; // et on quitte la fonction
  }
  char *tmp = ltos(L); // on convertit la liste en une chaîne de caractère
  fprintf(f, "%s", tmp); // on écrit cette chaîne de caractère dans le fichier

  fclose(f); // on ferme le fichier
  free(tmp);
}

// Transforme le contenu d'un fichier en une liste
// On suppose que la taille de la chaîne contenue dans le fichier ne dépasse pas
// 256 caractères
List *ftol(char *path) {
  // Alloc

  FILE *f = fopen(path, "r"); // on ouvre le fichier en mode lecture
  if (f == NULL){
    printf("Erreur lors de l'ouverture du fichier %s\n", path); // si ça echoue on affiche un message d'erreur
    return NULL; // et on retourne NULL
  }
  char buffer[256]; // on initialise une chaîne de caractère de taille 256
  fgets(buffer, 256, f); // on copie dedans le contenu du fichier, qui ici est supposé en une ligne
  List *L = stol(buffer); // puis on transforme cette chaîne en une Liste

  fclose(f); // on ferme le fichier
  return L; // et on retourne la liste
}
