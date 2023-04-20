#include "commit.h"
#include "LC.h"
#include "directory.h"
#include "hachage.h"
#include "worktree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Taille de la table de hachage
#define TAILLE 10

// Crée un couple clé valeur
kvp *createKeyVal(char *key, char *val) {
  // Alloc
  kvp *kv = (kvp *)malloc(sizeof(kvp));
  if (kv == NULL) {  // Test si la réservation mémoire réussie
    return NULL;
  }
  kv->key = strdup(key);  // Affectation des champs en n'oubliant pas de dupliquer ceux-ci
  kv->value = strdup(val);
  return kv;
}

// Libère un couple clé valeur
void freeKeyVal(kvp *kv) {
  if (kv != NULL) {
    if (kv->key != NULL) { // On test si les champs sont bien présents
      free(kv->key);
    }
    if (kv->value != NULL) {
      free(kv->value);
    }
    free(kv);
  }
}

// Transforme un key_value_pair en chaîne de caractères
char *kvts(kvp *k) {
  // Alloc
  if (k == NULL)
    return NULL;
  int taille = strlen(k->key) + strlen(k->value) + 2; // On calcule la taille associée
  char *chaine = (char *)malloc(taille * sizeof(char)); // On réserve la zone mémoire
  if (chaine == NULL) {
    return NULL;
  }
  strcpy(chaine, k->key); // On copie la clé dans chaine
  strcat(chaine, ":");  // Puis on concatène : et la valeur
  strcat(chaine, k->value);
  return chaine;
}

// Lis un key_value_pair dans une chaîne de caractères
// On suppose que les clé et les valeurs ne dépassent pas 256 caractères
// On suppose que str a le bon format
kvp *stkv(char *str) {
  // Alloc
  if (str == NULL)
    return NULL;
  int i = 0; // Rang dans le tableau des clés et dans la chaîne str
  int j = 0; // Rang dans le tableau des valeurs
  char key[256];
  char val[256];

  // Récupération de la clé
  while (str[i] != ':') { // Tant que l'on traite la clé
    key[i] = str[i]; // On copie la clé dans le tableau key
    i++;
  }
  key[i] = '\0'; // On ajoute le \0
  i++; // On passe à la valeur

  // Récupération de la valeur
  while (str[i] != '\0') { // Tant que l'on est pas à la fin de la chaîne
    val[j] = str[i]; // On copie la valeur dans le tableau val
    j++;
    i++;
  }
  val[j] = '\0'; // On ajoute le \0
  kvp *kv = createKeyVal(key, val); // On crée le couple clé valeur
  return kv;
}

// Initialisation d'un commit
Commit *initCommit() {
  // Alloc
  Commit *c = (Commit *)malloc(sizeof(Commit));
  if (c == NULL) {
    return NULL;
  }
  c->n = 0; // Initialisation des champs de la structure
  c->size = TAILLE;
  c->T = (kvp **)malloc(sizeof(kvp *) * c->size); // On réserve la zone mémoire pour la table de hachage
  if (c->T == NULL) {
    free(c);
    return NULL;
  }
  int i;
  for (i = 0; i < c->size; i++) { // On met toutes les cases de la table à NULL
    c->T[i] = NULL;
  }
  return c;
}

// Libération d'un commit
void libererCommit(Commit *c) {
  if (c != NULL) {
    int i;
    for (i = 0; i < c->size; i++) { // On parcours la table
      freeKeyVal(c->T[i]); // On libère tous les couples clé valeur
    }
    free(c->T); // On libère la table
    free(c); // On libère le commit
  }
}

// fonction de hachage choisie djb2
unsigned long djb2(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

// Ajoute un nouvel élément à sa place dans la table de hachage
void commitSet(Commit *c, char *key, char *value) {
  if (c == NULL) {
    printf("Insertion impossible\n");
    return;
  }
  int i = 0; // Rang du probing linéaire
  int cle;
  kvp *kv;
  while (i < c->size) { // On augmente i jusqu'à trouver une case de libre, on s'arrête si i dépasse la taille de la table
    cle = (int)((djb2(key) + i) % c->size); // On calcule la case à l'aide de la fonction de hachage
    if (c->T[cle] == NULL) { // On regarde si la case est libre
      kv = createKeyVal(key, value); // Si elle l'est, on crée un couple clé valeur
      if (kv == NULL) {
        printf("Insertion impossible\n");
        return;
      }
      c->T[cle] = kv; // On place ce couple à la bonne case
      c->n++; // On augmente le nombre d'éléments
      return;
    }
    i++; // Sinon au augmente i et on re-calcule un case
  }
  printf("Insertion impossible, la table est pleine\n");
}

// Création d'un commit avec le renseignement du champs tree
Commit *createCommit(char *hash) {
  // Alloc
  Commit *c = initCommit(); // On initialise un commit
  if (c == NULL)
    return NULL;
  commitSet(c, "tree", hash); // On renseigne le champs tree qui est le hash du WorkTree associé au commit
  return c;
}

// Récupère un élément d'un commit dont la clé est key, ne duplique pas le key_value_pair
char *commitGet(Commit *c, char *key) {
  int i = 0; // Rang du probing linéaire
  int cle;
  while (i < c->size) { // On continue tant que l'on a pas trouvé la case ou si on dépasse la taille de la table
    cle = (djb2(key) + i) % c->size; // On calcule la case où l'élément devrait être
    if (c->T[cle] != NULL) { // Si la case n'est pas vide
      if (strcmp((c->T[cle])->key, key) == 0) { // On compare les clés
        return (c->T[cle])->value; // On retourne l'élément tel quel
      }
    }
    i++; // Sinon on continue la recherche
  }
  return NULL; // L'élément n'y est pas
}

// Transforme un commit en chaîne de caractères
char *cts(Commit *c) {
  if (c == NULL)
    return NULL;
  int i; // Rang dans la table

  // Calcul de la taille à réservée
  int taille = 0;
  for (i = 0; i < c->size; i++) { // On parcours la table pour calculer précisément la taille du bloc à réserver
    if (c->T[i] != NULL) {
      taille = taille + strlen(c->T[i]->key) + strlen(c->T[i]->value) + 2; // taille(clé) + taille(value) + taille(:) + taille(\n)
    }
  }
  taille = taille + 1; // Pour le \0
  
  char *str = (char *)malloc(taille * sizeof(char));
  if (str == NULL)
    return NULL;
  i = 0;
  strcpy(str, ""); //On initialise la chaîne str 
  for (i = 0; i < c->size; i++) { // On parcours la table
    if (c->T[i] != NULL) { // S'il y a un élément dans cette case
      char *kv = kvts(c->T[i]); // On obtient la chaîne de caractères associée à l'élément de la case
      strcat(str, kv); // On la concatène à str
      strcat(str, "\n"); // On ajoute le \n
      free(kv);
    }
  }
  return str;
}

// Lis un commit depuis une chaîne de caractères
// On suppose que ch respecte bien la structure d'un key_value_pair
Commit *stc(char *ch) {
  if (ch == NULL)
    return NULL;
  Commit *c = initCommit(); // On initialise un commit
  if (c == NULL) {
    return NULL;
  }
  kvp *kv;
  char buffer[256];
  int i = 0, j = 0; // i rang de ch et j rang dans le buffer
  while (ch[i]) { // On parcours la chaîne ch
    if (ch[i] == '\n') { // Si l'on atteint la fin d'un couple
      buffer[j] = '\0'; // On ajoute un \0 au buffer
      kv = stkv(buffer); // On le convertie en couple clé valeur
      commitSet(c, kv->key, kv->value); // On l'ajoute à la table
      freeKeyVal(kv);
      j = 0; // On réinitialise le rang du buffer
      i++;
    } 
    else { // Sinon on remplie le buffer
      buffer[j] = ch[i];
      j++;
      i++;
    }
  }
  return c;
}

// Ecris un commit dans un fichier
void ctf(Commit *c, char *file) {
  FILE *f = fopen(file, "w"); // On ouvre le fichier
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", file);
    return;
  }
  char *s = cts(c);
  fprintf(f, "%s", s); // On y place la chaîne de caractères représentant un commit
  free(s);
  fclose(f);
}

// Lit un commit depuis un fichier
// On suppose ques les lignes du fichier respectent bien la structure d'un key_value_pair et ne dépassent pas 256 caractères
Commit *ftc(char *file) {
  // Alloc
  Commit *c = initCommit(); // On initialise un commit
  if (c == NULL) {
    return NULL;
  }
  FILE *f = fopen(file, "r"); // On ouvre le fichier
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", file);
    libererCommit(c);
    return NULL;
  }
  
  char buffer[256]; // Contient les lignes du fichier
  kvp *kv;
  int i;
  char *res = fgets(buffer, 256, f); // On lit la première ligne
  while (res) { // On parcours tout le fichier
    // Ajout du \0
    for (i = 0; buffer[i] != '\n'; i++) {
    }
    buffer[i] = '\0';
    
    kv = stkv(buffer); // On convertie la chaîne en couple clé valeur
    commitSet(c, kv->key, kv->value); // On l'ajoute à la table
    freeKeyVal(kv);
    res = fgets(buffer, 256, f); // On lit la ligne suivante
  }
  fclose(f);
  return c;
}

// Crée un enregistrement instantané d'un commit
char *blobCommit(Commit *c) {
  // Alloc
  
  // création du fichier temporaire
  char tmp[103];
  char *filename = (char *)malloc(100 * sizeof(char));
  strcpy(filename, "tmp");
  sprintf(tmp, "%sXXXXXX", filename); // On place dans tmp le nom du fichier temporaire
  int id = mkstemp(tmp); // On crée le fichier temporaire
  if (id == -1) {
    printf("Création du fichier tmp echoué\n");
    return NULL;
  }

  //écriture du contenu dans le fichier tmp
  ctf(c, tmp);

  // hachage du contenu de tmp
  char *hash = sha256file(tmp);

  // enregistrement du fichier instantané à partir de tmp
  char *path = hashToPath(hash); // On obtient le chemin du nouveau fichier
  char *chemin = (char *)malloc(sizeof(char) * (strlen(path) + 3));
  strcpy(chemin, path);
  char dir[3] = {chemin[0], chemin[1], '\0'};
  if (!file_exists(dir)) { // On crée le répertoire nommé par les deux premières lettres du hash s'il n'existe pas
    char template[10];
    sprintf(template, "mkdir %s", dir);
    system(template);
  }
  char extension[3] = {'.', 'c', '\0'};
  strcat(chemin, extension); // On ajoute l'extension au chemin
  cp(tmp, chemin); // On copie les éléments du fichier temporaire vers notre nouveau fichier

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

// Initialisation des références
void initRefs() {
  List *l = listdir(".refs"); // On liste les éléments du dossier .refs/
  if (*l == NULL) { // Si la liste est vide
    system("mkdir .refs"); // On crée le dossier
    system("touch .refs/master"); // On crée les fichiers master et HEAD
    system("touch .refs/HEAD");
  } else { // Sinon
    Cell *cell1 = searchList(l, "master"); // On cherche le fichier master
    if (cell1 == NULL) // S'il n'existe pas, on le crée
      system("touch .refs/master");
    Cell *cell2 = searchList(l, "HEAD"); // On cherche le fichier HEAD
    if (cell2 == NULL) // s'il n'existe pas, on le crée
      system("touch .refs/HEAD");
  }
  libererList(l);
}

// Modifie le contenu d'une référence par hash
void createUpdateRef(char *ref_name, char *hash) {
  char fichier[80]; // Contient le chemin vers la référence ref_name
  sprintf(fichier, ".refs/%s", ref_name);
  FILE *f = fopen(fichier, "w"); // On ouvre le fichier
  if (f == NULL) { // Si l'ouverture echoue on renvoie Null et un message d'erreur
    printf("Erreur lors de l'ouverture du fichier %s\n", fichier);
  }
  fprintf(f, "%s", hash); // On y écrit hash
  fclose(f); // fermeture du fichier
}

// Supprime la référence dont le nom est passé en paramètre
void deleteRef(char *ref_name) {
  List *l = listdir(".refs"); // On liste les fichiers existants dans le répertoire .refs/
  if (*l == NULL) { // Si la liste est vide, on s'arrête car il ne contient pas le fichier ref_name
    libererList(l);
    return;
  }
  Cell *cell = searchList(l, ref_name); // Sinon on recherche le fichier ref_name
  if (cell) { // Si on le trouve
    char buffer[100];
    sprintf(buffer, "rm .refs/%s", ref_name);
    system(buffer); // On le supprime
  }
  libererList(l);
}

// Récupère le hash contenu dans la référence ref_name
char *getRef(char *ref_name) {
  // Alloc
  char fichier[256]; // Contient le chemin vers le fichier ref_name
  sprintf(fichier, ".refs/%s", ref_name);
  FILE *f = fopen(fichier, "r"); // On ouvre le fichier
  if (f == NULL){ // si l'ouverture echoue on renvoie un message d'erreur et retourne Null
    printf("Erreur lors de l'ouverture du fichier %s\n", ref_name);
    return NULL;
  }
  char buffer[256]; // Contient le contenu du fichier
  char *res = fgets(buffer, 256, f); // On lit la première et seule ligne

  // Si le fichier est vide, on retourne la chaîne vide
  if (res == NULL) {
    char *chaine = (char *)malloc(sizeof(char));
    chaine[0] = '\0';
    fclose(f);
    return chaine;
  }

  // Sinon le contenu
  char *chaine = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
  strcpy(chaine, buffer);
  fclose(f);
  return chaine; // on retourne la chaine contenant le contenu du fichier ref_name
}

// Ajoute un fichier ou un répertoire au fichier .add
void myGitAdd(char *file_or_folder) {
  if (file_or_folder == NULL) {
    return;
  }
  WorkTree *wt;
  List *l = listdir("."); // On liste les fichiers et répertoires du répertoire courant
  Cell *cell = searchList(l, ".add"); // On y cherche le fichier .add
  int retour = 0;

  // Si le fichier .add n'existe pas, on crée un nouveau WorkTree
  if (cell == NULL) {
    wt = initWorkTree(); // initialisation du WorkTree
    if (wt == NULL) { // Si l'allocation echoue on affiche un message d'erreur et quitte la fonction
      printf("Erreur d'insertion myGitAdd"); // 
      libererList(l);
      return;
    }
  }

  // Sinon on charge le WorkTree associé au fichier .add
  else {
    wt = ftwt(".add"); // on transforme ".add" en WorkTree à l'aide la fonction ftwt (fichier->WorkTree)
    if (wt == NULL) { // Si la transformation echoue
      printf("Erreur d'insertion myGitAdd"); // on renvoie un message d'erreur et quitte la fonction
      libererList(l);
      return;
    }
  }
  // on regarde si le fichier en question existe
  if (file_exists(file_or_folder)) {
    // On ajoute le nom du fichier ou répertoire au WorkTree
    // Puis on replace le WorkTree dans le fichier .add
    appendWorkTree(wt, file_or_folder, "hash", 420);
    retour = wttf(wt, ".add");
  }
  else{
    // S'il n'existe pas, alors on renvoie un message d'erreur
    printf("Le fichier %s n'existe pas\n", file_or_folder);
  }

  if (retour == -1) { // si retour est à -1, alors il y'a eu un echec lors de l'insertion et on affiche le message d'erreur
    printf("Erreur d'insertion myGitAdd");
  }
  libererWorkTree(wt);
  libererList(l);
}


// Réalise un commit
void myGitCommit(char *branch_name, char *message) {
  
  // Test si les références ont été initialisées
  List *l = listdir(".refs"); // on obtient la liste des branches de ".refs"
  if (*l == NULL) { // si la liste est null
    printf("Initialiser d'abord les références du projet\n"); // on envoie un message d'erreur
    return; // puis on quitte la fonction
  }
  libererList(l);

  // Récupère les contenus de branch_name et HEAD
  char *contenu_branch_name = getRef(branch_name); // on récupère la référence de la branche
  if (contenu_branch_name == NULL) { // si on echoue cela signifie que la branche n'existe pas
    printf("La branche n'existe pas\n"); // donc on renvoie un message d'erreur et on quitte la fonction
    return;
  }
  char *contenu_head = getRef("HEAD"); // on récupère aussi la référence de HEAD
  if (contenu_head == NULL) { // si la référence de HEAD est null
    printf("Initialiser d'abord les références du projet\n"); // on renvoie un message d'erreur et on quitte
    free(contenu_branch_name);
    return;
  }

  if (strcmp(contenu_branch_name, contenu_head) != 0) { // on vérifie que la référence de HEAD et branch_name sont identiques
    printf("HEAD doit pointer sur le dernier commit de la branche\n"); // Si ce n'est pas le cas, on renvoie un message d'erreur et on quitte
    free(contenu_head);
    free(contenu_branch_name);
    return;
  }

  // Sinon on récupère le contenu du fichier .add puis on supprime ce fichier
  WorkTree *wt = ftwt(".add"); // on transforme ".add" en un WorkTree
  if (wt == NULL) { // Si la transformation echoue, on renvoie un message d'erreur et on quitte
    printf("Erreur myGitCommit\n");
    free(contenu_head);
    free(contenu_branch_name);
    return;
  }
  system("rm .add"); // suppression du fichier ".add"

  // Sauvegarde le WorkTree associé
  // Puis crée le commit et renseigne ses champs
  char *hashWorkTree = saveWorkTree(wt, "."); // on sauvargde le WorkTree obtenu à partir de ".add"
  Commit *c = createCommit(hashWorkTree); // on créer un nouveau Commit dont le hash du tree est celui du WorkTree sauvegardé avant
  if (strlen(contenu_branch_name) != 0) { //Si le contenu de branch_name est pas null
    commitSet(c, "predecessor", contenu_branch_name); // on ajoute au predecessor du Commit, l'ancien dernier commit de la branche, c'est à dire la référence de branch name
  }
  if (message != NULL) { // si le message donné en paramètre n'est pas null
    commitSet(c, "message", message); // on l'ajoute au Commit
  }
  char *hashCommit = blobCommit(c); // puis on créer un enregistrement instantané du Commit

  // Modifie les références
  createUpdateRef(branch_name, hashCommit); // on met à jour la référence de branch_name
  createUpdateRef("HEAD", hashCommit); // Et celle de HEAD

  // Libération
  libererWorkTree(wt);
  free(contenu_head);
  free(contenu_branch_name);
  free(hashWorkTree);
  free(hashCommit);
  libererCommit(c);
}
