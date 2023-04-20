#include "directory.h"
#include "LC.h"
#include "hachage.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Le tilde n'est pas reconnu car specifique au shell
// fonction qui met le contenu de root_dir dans une liste
List *listdir(char *root_dir) {
  // Alloc
  List *l = initList(); // on créer une Liste
  Cell *c; 
  DIR *dp = opendir(root_dir); // on ouvre le répertoire root_dir
  struct dirent *ep;
  if (dp != NULL) { // si le répertoire n'est pas null
    while ((ep = readdir(dp)) != NULL) { // on parcours le contenu du répertoire
      // ici on fait des test pour ne pas récupérer . et .. dans la liste
      if (strcmp(ep->d_name, ".") != 0) {
        if (strcmp(ep->d_name, "..") != 0) {
          c = buildCell(ep->d_name); // puis on les ajoute un à un dans la liste
          insertFirst(l, c);
        }
      }
    }
  }
  closedir(dp); // on ferme le répertoire
  return l; // et on renvoie la liste
}

// fonction qui teste si un fichier existe dans le répértoire courant
// en utilisant les fonctions searchlist et listdir définit auparavant
// renvoie 1 s'il existe 0 sinon
int file_exists(char *file) {
  List *l = listdir("."); // on obtient l'ensemble des fichiers/répertoire du dossier courant dans une liste
  int n = searchList(l, file) ? 1 : 0; // on utilises searchList pour voir la présence du fichier/répertoire
  libererList(l);
  return n; // on retourne le résultat, 1 s'il existe et 0 sinon
}

// copie le contenu de "from" et l'écrit dans "to"
void cp(char *from, char *to) {

  // Ouverture
  FILE *f = fopen(from, "r"); // ouverture du fichier 'from' en mode lecture
  if (f == NULL) { // si l'ouverture echoue en affiche un message d'erreur puis on quitte
    printf("Erreur lors de l'ouverture du fichier: %s\n", from);
    return;
  }
  FILE *t = fopen(to, "w"); // ouverture du fichier 'to' en mode écriture
  if (t == NULL) { // si l'ouverture echoue en affiche un message d'erreur puis on quitte 
    fclose(f);
    printf("Erreur lors de l'ouverture du fichier: %s\n", to);
    return;
  }

  char buffer[256]; // on créer un buffer 
  char *res = fgets(buffer, 256, f); // on lit dans le fichier "from"
  // lecture ligne par ligne
  while (res) { // tant qu'on peut lire les lignes dans le fichier "from"
    fprintf(t, "%s", buffer);  // on les écrit dans le fichier "to"
    res = fgets(buffer, 256, f);
  }

  fclose(f); // fermeture des deux fichiers
  fclose(t);
}

// Transforme un hash en un path en rajoutant
// un "/" entre le 2ème et 3ème caractère
char *hashToPath(char *hash) {
  // Alloc
  char *chemin = (char *)malloc(100 * sizeof(char)); // on alloue une nouvelle chaîne de caractère
  chemin[0] = hash[0];
  chemin[1] = hash[1]; // on récupère les deux premieres caractères qui sont le répertoire
  chemin[2] = '/'; // puis on ajoute un slash après
  int i = 2;
  while (hash[i] != '\0') { // et parcours le restant du hash, pour ajouter les caractères restants
    chemin[i + 1] = hash[i];
    i++;
  }
  chemin[i + 1] = '\0';
  return chemin; // on retourne le chemin
}

// créer un enregistrement instantané d'un fichier
// on obtient son hash et on crée le répértoire associé
// s'il n'existe pas puis on copie le contenu de file dans
// l'enregistrement instantanée
void blobFile(char *file) {
  char *hash = sha256file(file); // on obtient le hash du fichier
  char *chemin = hashToPath(hash); // on transforme le hash en un chemin
  char dir[3] = {chemin[0], chemin[1], '\0'}; // on verifie l'existence du répertoire
  if (!file_exists(dir)) { // s'il n'existe pas on le créer
    char template[10];
    sprintf(template, "mkdir %s", dir);
    system(template);
  }
  cp(file, chemin); // puis on copie le contenu du fichier d'origine dans son enregistrement instantané

  free(hash);
  free(chemin);
}
