#ifndef LC_H_
#define LC_H_

// Structure Cell
typedef struct cell {
  char *data;
  struct cell *next;
} Cell;

typedef Cell *List;

// Initialisation d'une liste vide
List *initList();

// Création d'une cellule
Cell *buildCell(char *ch);

// Insertion d'une cellule en tête d'une liste, ne duplique pas la cellule
void insertFirst(List *L, Cell *C);

// Libération d'une cellule
void libererCell(Cell *c);

// Libération d'une liste
void libererList(List *l);

// Transforme une cellule en chaîne de caractères
char *ctos(Cell *c);

// Transforme un liste en chaîne de caractères
// C'est à dire l'ensemble des Cell qu'il contient séparé par des "|"
char *ltos(List *L);

// Retourne le i-ème élément de la liste, ne fait pas de duplication
// On suppose que i < len(L)
Cell *listGet(List *L, int i);

// Cherche l'élément dont le champs name est égal à str dans la liste
// Ne duplique pas la cellule
Cell *searchList(List *L, char *str);

// Transforme une chaîne de caractères représentant un List en une liste
// on sait qu'une List est un ensemble de Cell séparé par des "|"
// donc il suffit de parcourir la chaîne de caractère
List *stol(char *s);

// Ecris la représentation sous forme de chaîne de caractère d'une liste dans un fichier
void ltof(List *L, char *path);

// Transforme le contenu d'un fichier en une liste
// On suppose que la taille de la chaîne contenue dans le fichier ne dépasse pas
// 256 caractères
List *ftol(char *path);



#endif
