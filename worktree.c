#include "worktree.h"
#include "directory.h"
#include "hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Nombre de WorkFile maximum dans un WorkTree
#define TAILLE 10

// Création d'un WorkFile
WorkFile *createWorkFile(char *name) {
  // Alloc

  WorkFile *wf = (WorkFile *)malloc(
      sizeof(WorkFile)); // malloc pour alloué de la mémoire à un WorkFile
  if (wf == NULL)
    return NULL;
  wf->name = strdup(name); // on duplique le nom à l'aide de strdup
  wf->hash = NULL;         // on met la hash à NULL
  wf->mode = 0;            // on met le mode à 0
  return wf;               // on retourne le WorkFile
}

// Libération d'un WorkFile
void libererWorkFile(WorkFile *wf) {
  if (wf) {
    free(wf->name); // on libère le name
    if (wf->hash) {
      free(wf->hash); // puis le hash s'il existe
    }
    free(wf); // et finalement le WorkFile
  }
}

// Transformation d'un WorkFile en chaîne de caractères
// On suppose que strlen(wf->name + wf->hash + wf->mode + 2) < 256
char *wfts(WorkFile *wf) {
  // Alloc
  char *s = (char *)malloc(
      sizeof(char) * 256); // on alloue un chaine de caractère de taille 256
  if (s == NULL)
    return NULL; // Si l'alloc echoue on renvoi Null
  sprintf(s, "%s\t%s\t%d", wf->name, wf->hash, wf->mode); // on met tous les champs dans la chaîne à l'aide de sprintf (chaque champs étant séparé par des "\t");
  return s;
}

// Tranformation d'une chaîne de caractères en un WorkFile
// On suppose que les champs name et hash ne dépassent pas 256 caractères
WorkFile *stwf(char *ch) {
  // Alloc
  char name[256]; // chaine statique pour le champ name
  char hash[256]; // chaine statique pour le champ hash
  int mode;
  WorkFile *wf = (WorkFile *)malloc(
      sizeof(WorkFile)); // on alloue la mémoire pour un WorkFile
  if (wf == NULL)
    return NULL; // Si l'alloc echoue en renvoie Null
  sscanf(ch, "%s\t%s\t%d", name, hash,
         &mode);           // on récupère chaque champ à l'aide de sscanf
  wf->name = strdup(name); // on les duplique un à un dans la structure WorkFile
  wf->hash = strdup(hash);
  wf->mode = mode;
  return wf;
}

// Initialisation d'un WorkTree
WorkTree *initWorkTree() {
  // Alloc
  WorkTree *wkt = (WorkTree *)malloc(
      sizeof(WorkTree)); // On alloue la mémoire pour un WorkTree
  if (wkt == NULL)
    return NULL; // Si l'alloc echoue en renvoie Null
  wkt->tab = (WorkFile *)malloc(
      TAILLE *
      sizeof(WorkFile)); // on alloue la mémoire pour la tableau de WorkFile
                         // dont la taille est définit par la constante TAILLE
  wkt->size = TAILLE;    // on donne à size sa TAILLE
  wkt->n = 0; // et initialise le nombre d'élément dans le tableau, c'est à dire 0 au début
  return wkt; // et on renvoie le WorkTree
}

// Libération d'un WorkTree
void libererWorkTree(WorkTree *wkt) {
  if (wkt !=
      NULL) { // on vérifie que le WorkTree n'est pas Null avant de le free
    for (int i = 0; i < wkt->n;
         i++) { // on free chacun des WorkFiles contenu dans le WorkTree
      free(wkt->tab[i].name); // on verifie que ses champs name et hash sont pas null avant de les free
      if (wkt->tab[i].hash) {
        free(wkt->tab[i].hash);
      }
    }
    free(wkt->tab); // on free le tableau de WorkFile
    free(wkt);      // et on finit par la structure
  }
}

// Test de l'existence d'un WorkFile de wt tel que son champs name est égal à
// name Retourne son indice dans le tableau ou -1 si il n'y est pas
int inWorkTree(WorkTree *wt, char *name) {
  int i;
  for (i = 0; i < wt->n;
       i++) { // on parcours le tableau de WorkFile du WorkTree
    if (wt->tab[i].name != NULL &&
        name != NULL) { // on vérifie d'abord que name et tab[i].name sont
                        // différents de null
      if (strcmp(wt->tab[i].name, name) ==
          0) {    // puis on les compare à l'aide de strcmp
        return i; // s'il sont identiques alors on retourne sont indice dans le tableau
      }
    }
  }
  return -1; // on retourne -1 car on a parcouru tout le tableau, donc le
             // WorkFile n'y est pas
}

// Ajoute un WorkFile au WorkTree
// On suppose que les champs ne valent pas NULL
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode) {
  // Alloc
  if ((inWorkTree(wt, name) == -1) &&
      (wt->n < wt->size)) { // on vérifie d'abord que le WorkFile n'est pas déjà
                            // dans dedans et qu'il y a suffisament de place
                            // pour l'ajouter dans le tableau
    int n = wt->n;
    wt->tab[n].name =
        strdup(name); // Si c'est la cas on initialise chacun des champs du
                      // WorkFile contenu dans le WorkTree
    wt->tab[n].hash = strdup(hash);
    wt->tab[n].mode = mode;
    wt->n = wt->n + 1; // et on met à jour le nombre de WorkFile du tableau
    return 0;          // retourne 0 si l'ajout a été effectué
  }
  return -1; // sinon retourne -1
}

// Transforme un WorkTree en chaîne de caractères
// On suppose que strlen(name + hash + mode) < 256 pour chaque WorkFile
char *wtts(WorkTree *wt) {
  // Alloc

  // Si le WorkTree est vide
  if (wt->n == 0) {
    char *s =
        (char *)malloc(sizeof(char)); // on alloue une place pour la chaine vide
    if (s == NULL)
      return NULL;   // si l'alloc echoue on renvoie null
    strcpy(s, "\0"); // on met le caractère fin de chaîne de caractère
    return s;        // on retourne cette chaîne vide
  }
  // Si le WorkTree n'est pas vide
  char *s = (char *)malloc((257 * (wt->n)) *
                           sizeof(char)); // On alloue suffisament de place pour
                                          // chacun des WorkFile, ici 256
  if (s == NULL)
    return NULL; // si l'alloc echoue on retourne null
  int i, j;
  int c = 0;
  char *wf;
  for (i = 0; i < wt->n; i++) { // on parcours les WorkFile du tableau
    wf =
        wfts(&(wt->tab[i])); // on transforme le WorkFile en chaîne de caractère
    if (wf == NULL) { // si l'un d'entre eux echoue on renvoie null et arrête la
                      // fonction
      free(s);
      return NULL;
    }
    // On ajoute un à un les caractères dans s
    for (j = 0; wf[j]; j++) {
      s[c] = wf[j];
      c++;
    }
    free(wf);
    s[c] = '\n'; // puis on ajoute le retour à la ligne avant de passer au prochain WorkFile
    c++;
  }
  s[c] = '\0'; // Quand on a ajouté tous les WorkFile on rajoute le caractère de fin à la chaîne finale
  return s; // on retourne la représentation du WorkTree sous forme de chaîne de caractère
}

// Transforme une chaîne de caractères en un WorkTree
WorkTree *stwt(char *s) {
  // Alloc

  // Initialisation
  WorkTree *wt = initWorkTree(); // on créer le WorkTree
  if (wt == NULL)
    return NULL;// si la création echoue on renvoie Null
  WorkFile *wf;
  char ch[256];
  int c = 0;
  int i = 0;

  // Parcours de la chaîne s
  while (s[c] != '\0') { 
    while (s[c] && (s[c] != '\n')) { // Tant qu'on ne rencontre pas la caractère "\n" (qui permet de séparer les WorkFile)
      ch[i] = s[c]; // on met le caractère dans la chaîne
      c++; // on passe au caractère suivant dans s
      i++; // on passe au caractère suivant dans ch
    }
    ch[i] = '\0'; // on sort du while, donc on a rencontré un retour à la ligne
    i = 0; // on remet le compteur pour la chaine ch à zero
    c++; // on passe au caractère suivant dans s
    wf = stwf(ch); // on transforme ch qui est une chaine de caractère représentant un WorkFile en WorkFile
    if (wf == NULL) {
      libererWorkTree(wt); // Si la transformation echoue, on renvoie Null
      return NULL;
    }
    if (wt->n >= wt->size) { // on regarde si on peut ajouter le WorkFile dans le WorkTree, (la taille étant limité)
      libererWorkTree(wt);
      libererWorkFile(wf); // Si l'on peut pas, on sort de la fonction et on retourne Null
      return NULL;
    }
    appendWorkTree(wt, wf->name, wf->hash, wf->mode); // Sinon on l'ajoute au WorkTree
    libererWorkFile(wf);
  }
  return wt; // Quand la boucle while principal est terminé, cela veut dire qu'on parcouru toute la chaine de caractère, donc on peut renvoyer le WorkTree.
}

// Ecris la représenation sous forme de chaîne de caractère d'un WorkTree dans un fichier nommé file
int wttf(WorkTree *wt, char *file) {
  FILE *f = fopen(file, "w"); // on ouvre le fichier en mode écriture
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", file); // Si l'ouverture echoue en envoie un message d'erreur et retourne -1
    return -1;
  }
  char *s = wtts(wt); // on transforme le WorkTree en chaîne de caractère à l'aide de la fonction wtts
  if (s == NULL) {
    fclose(f); // Si cela echoue on renvoie -1
    return -1;
  }
  fprintf(f, "%s", s); // Sinon on écrit cette chaîne dans le fichier
  free(s);
  fclose(f); // on oublie pas de fermer le fichier
  return 0; // et on retourne 0
}

// Lis un WorkTree depuis un fichier
// Et le transforme en un WorkTree
// Prérequis, il faut que le fichier soit écrit dans le format d'un WorkTree
// C'est à dire qu'elle contient la représentation sous forme de chaîne de caractère d'un WorkFile par ligne
WorkTree *ftwt(char *file) {
  // Alloc
  FILE *f = fopen(file, "r"); // on essaie d'ouvrir le fichier en mode lecture
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", file); // si ça echoue on renvoie un message d'erreur et retourne Null
    return NULL;
  }
  char buffer[256]; // on initialise un buffer
  WorkTree *wt = initWorkTree(); // on créer un WorkTree
  if (wt == NULL)
    return NULL; // si la création du WorkTree echoue en renvoie Null
  WorkFile *wf; // on déclare un WorkFile

  // Parcours ligne par ligne du fichier
  while (fgets(buffer, 256, f)) {
    wf = stwf(buffer); // on transforme la ligne lue en WorkFile
    if (wf == NULL) {
      libererWorkTree(wt); // si la transformation echoue on ferme le fichier 
      fclose(f); // puis on renvoie Null
      return NULL;
    }
    if (wt->n >= wt->size) { // Sinon on regarde si on peut ajouter le WorkFile dans le WorkTree
      libererWorkTree(wt);
      libererWorkFile(wf); // Si on ne peut pas, alors on retourne Null
      fclose(f);
      return NULL;
    }
    appendWorkTree(wt, wf->name, wf->hash, wf->mode); // Sinon on l'ajoute au WorkTree
    libererWorkFile(wf);
  }
  fclose(f);
  return wt; // on a fini de lire tout le fichier, donc on renvoie le WorkTree
}


// Créer l'enregistrement instantané d'un WorkTree
char *blobWorkTree(WorkTree *wt) {
  // Alloc

  // création du fichier temporaire
  char tmp[100];
  char *filename = (char *)malloc(100 * sizeof(char)); // on alloue de la mémoire pour le nom du fichier temporaire
  strcpy(filename, "test"); // on le nomme fichier
  sprintf(tmp, "%sXXXXXX", filename); // on rajoute XXXXXX à la fin du nom du fichier pour la fonction mkstemp qui les remplacer
  int id = mkstemp(tmp); // on créer un fichier temporaire nommée testXXXXXX, où XXXXXX va être remplacé par une chaîne unique
  if (id == -1) {
    printf("Création du fichier tmp echoué\n"); // si la création du fichier temporaire echoue en renvoie Null
    return NULL;
  }

  //écriture du contenu dans le fichier tmp
  if (wttf(wt, tmp) == -1) { // on transforme le WorkTree en file, qui pour nom celui du fichier tmp (en d'autre terme on écrit sa représentation dans le fichier temporaire)
    printf("échec dans l'écriture\n"); // si cela echoue on renvoie Null
    return NULL;
  }

  // hachage du contenu de tmp
  char *hash = sha256file(tmp); // on obtient le hash du fichier temporaire

  // enregistrement du fichier instantané à partir de tmp
  char *path = hashToPath(hash); // on transforme ce hash en chemin à l'aide de la fonction hashToPath
  char *chemin = (char *)malloc(sizeof(char) * (strlen(path) + 3)); // on alloue de la mémoire pour une nouvelle chaîne de caractère 
  strcpy(chemin, path); // celle ci va contenir le hash sous forme de chemin concaténer à l'extension .t
  char dir[3] = {chemin[0], chemin[1], '\0'};
  if (!file_exists(dir)) { // on vérifie l'existence du répertoire où va contenir l'enregistrement instantané
    char template[10];
    sprintf(template, "mkdir %s", dir); // s'il n'existe pas on le créer
    system(template);
  }
  char extension[3] = {'.', 't', '\0'}; // ici on rajoute l'extension .t 
  strcat(chemin, extension);
  cp(tmp, chemin); // on copie le contenu du fichier tmp dans fichier qui a pour nom le hash du WorkTree

  // suppression du fichier temporaire
  char nom[120];
  sprintf(nom, "rm %s", tmp);
  system(nom);

  // on oublie pas de free bien evidemment
  free(chemin);
  free(filename);
  free(path);

  // valeur de retour: hash du fichier tmp
  return hash;
}

// Fonction getChmod donné dans l'énoncé
// remarque ici le mode est donné sous forme décimale
int getChmod(const char *path) {
  struct stat ret;

  if (stat(path, &ret) == -1) {
    return -1;
  }

  return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) |
         (ret.st_mode & S_IXUSR) | /*
owner*/
         (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) |
         (ret.st_mode & S_IXGRP) | /*
group*/
         (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) |
         (ret.st_mode & S_IXOTH); /*
other*/
}


// Réalise un enregistrement instantané du WorkTree et de tout son contenu
// on suppose que l'on met seulement le nom du répertoire sans le slash final
char *saveWorkTree(WorkTree *wt, char *path) {
  WorkFile *tab = wt->tab; // on récupère le tableau de WorkFile
  int i;

  // Permet de tester si on traite actuellement un fichier
  struct stat *ret = (struct stat *)malloc(sizeof(struct stat)); // structure stat, utile pour savoir s'il s'agit d'un fichier ou d'un répertoire
  for (i = 0; i < (wt->n); i++) {
    char name[256];
    sprintf(name, "%s/%s", path, tab[i].name); // on obtient le nom du WorkFile est le concatène avec le chemin donné en paramètre
    stat(name, ret);
    // test si c'est un fichier
    if (S_ISREG(ret->st_mode)) {
      // le cas où c'est un fichier
      blobFile(name); // on créer un enregistrement instantané de ce fichier avec blobFile
      tab[i].mode = getChmod(name); // on obtient son mode
      if (tab[i].hash != NULL) {
        free(tab[i].hash);
      }
      tab[i].hash = sha256file(name); // et on obtient son hash
    } else {
      // le cas où c'est un répértoire
      WorkTree *newWT = initWorkTree(); // on doit créer un nouveau WorkTree
      List *L = listdir(name); // on obtient le contenu de ce répertoire sous forme de list, avec la fonction listdir
      Cell *C = *L; // on créer une Cell qui contient cette liste
      while (C) { // on parcours la liste
        appendWorkTree(newWT, C->data, "(null)", getChmod(C->data)); // on ajoute chacun des éléments dans le WorkTree
        // par ailleurs le champs hash et mode sont pas important ici, puisqu'on les mettra à jour plus tard
        C = C->next; // on passe à l'élément suivant de la liste
      }
      // appel récursif sur newWT et path = name
      char *s = saveWorkTree(newWT, name);
      if (tab[i].hash != NULL)
        free(tab[i].hash);
      tab[i].mode = getChmod(name); // après l'appel récursif on met son mode à jour
      tab[i].hash = blobWorkTree(newWT); // et son hash à jour 

      free(s);
      libererWorkTree(newWT);
      libererList(L);
    }
  }
  free(ret);
  char *hash = blobWorkTree(wt); // on fait un enregistrement du WorkTree et on obitent son hash en même temps
  return hash; // on retourne le hash du WorkTree
}

// Modifie le mode
// on lit un mode qui est en décimale (base 10)
// et on le transforme en octale (base 8)
void setMode(int mode, char *path) {
  char buff[100];
  char oct[64];
  sprintf(oct, "%o", mode);
  sprintf(buff, "chmod %s %s", oct, path);
  system(buff);
}

// Restore un WorkTree
void restoreWorkTree(WorkTree *wt, char *path) {
  WorkFile *tab = wt->tab; // on obtient le talbeau de WorkFile
  int i;

  // Permet de tester l'existence de deux fichiers ayant le même nom mais l'un
  // terminant par .t
  int fic[2] = {0, 0}; // on créer un tableau de deux entier, 0 et 0
  for (i = 0; i < wt->n; i++) {
    fic[0] = 0;
    fic[1] = 0;
    char *chemin = hashToPath(tab[i].hash); // on obtient le chemin associé au hash du WorkFile

    // Test d'existence de l'enregistrement instantané
    FILE *f = fopen(chemin, "r"); // on regarde si le l'enregistrement instantané existe avec fopen
    if (f) {
      fic[0] = 1; // dans ce cas on met l'indice zéro du tableau fic à 1
    }
    char buffer[120]; // ensuite on va rajouter l'extension ".t" au chemin
    strcpy(buffer, chemin);
    strcat(buffer, ".t");
    FILE *t = fopen(buffer, "r"); // on regarde cette fois si l'enregistrement instantané existe
    if (t)
      fic[1] = 1; // s'il existe on met l'indice 2 du tableau fic à 1

    // Si le fichier n'existe pas 
    if ((fic[0] == 0) && (fic[1] == 0)) { // si les deux éléments du talbeau sont à 0 cela signifie que le l'enregistrement instantané n'existe pas
      printf("L'enregistrement instantané n'existe pas %s\n", chemin);
      free(chemin); // donc on envoie un message d'erreur et on arrête la fonction
      return;
    }

    // S'il en existe deux
    if ((fic[0] == 1) && (fic[1] == 1)) { // si les deux éléments du talbeau sont à 1, on alors une collision
      printf("Collision de l'enregistrement instantané %s\n", chemin);
      fclose(f); // on renvoie un message d'erreur et on arrête la fonction
      fclose(t);
      free(chemin);
      return;
    }

    // Dans ce cas on est dans un fichier
    if ((fic[0] == 1) && (fic[1] == 0)) { // si le premier élément du tableau est égal à 1 et le second à 0
      char template[150]; // on est dans le cas d'un fichier
      sprintf(template, "%s/%s", path, tab[i].name); // il suffit de concaténer le path au nom du fichier
      cp(chemin, template); // puis de copier le contenu de l'enregistrement instantané dans le fichier d'origine
      setMode(tab[i].mode, template); // puis de remettre ses droits
      fclose(f);
      free(chemin);
    }

    // Dans ce cas on est dans un répértoire
    if ((fic[0] == 0) && (fic[1] == 1)) { // si le premier élément du tableau est égal à 0 et le second à 1
      WorkTree *NewWT = ftwt(buffer); // on créer un nouveau WorkTree à partir de l'enregistrement instantanée
      char template[150];
      sprintf(template, "%s/%s", path, tab[i].name); // on concatene le path et nom du répertoire 
      restoreWorkTree(NewWT, template); // puis on fait un appel récursif sur ce WorkTree
      fclose(t);
      free(chemin);
      libererWorkTree(NewWT);
    }
  }
}
