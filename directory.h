#ifndef _directory_h_
#define _directory_h_

#include "LC.h"


// Le tilde n'est pas reconnu car specifique au shell
// fonction qui met le contenu de root_dir(répertoire) dans une liste
// c'est à dire l'ensemble de fichier/répertoire contenu dans root_dir dans une liste de Cell
List* listdir(char* root_dir);

// fonction qui teste si un fichier existe dans le répértoire courant
// en utilisant les fonctions searchlist et listdir définit auparavant
// renvoie 1 s'il existe 0 sinon
int file_exists(char *file);

// copie le contenu de "from" et l'écrit dans "to"
// on lit le fichier "from" ligne par ligne
// et chaque ligne qu'on lit, on l'écrit dans le fichier "to"
void cp(char *to, char *from);

// Transforme un hash en un path en rajoutant
// un "/" entre le 2ème et 3ème caractère
char* hashToPath(char* hash);

// créer un enregistrement instantané d'un fichier
// on obtient son hash et on crée le répértoire associé s'il n'existe pas
// puis on copie le contenu de file dans l'enregistrement instantanée
void blobFile(char* file);

#endif
