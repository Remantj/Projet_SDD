#include "LC.h"
#include "commit.h"
#include "directory.h"
#include "hachage.h"
#include "worktree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialise le fichier current_branch
// et met dans son contenu "master"
void initBranch() {
  FILE *f = fopen(".current_branch", "w");
  if (f == NULL) {
    printf("Erreur lors de la création du fichier .current_branch\n");
    return;
  }
  fprintf(f, "master");
  fclose(f);
}

// Test si une branche existe dans .refs
// renvoie 1 si elle existe 0 sinon
int branchExists(char *branch) {
  int retour;
  List *l = listdir(".refs"); // On regarde dans le répertoire .refs
  Cell *cell = searchList(l, branch); // recherche à l'aide de la fonction searchList
  if (cell == NULL) { // Si elle n'y est pas
    retour = 0;
  } else { // Si elle y est
    retour = 1;
  }
  libererList(l);
  return retour;
}

// créer une nouvelle branche
// il faut que HEAD existe
// et pointe bien vers un commit
void createBranch(char *branch) {
  char *hash = getRef("HEAD"); // On récupère la référence vers laquelle HEAD pointe
  // Si le fichier HEAD n'existe pas
  if (hash == NULL) { 
    printf("Le fichier HEAD n'existe pas\n");
    return;
  }
  // Si le fichier HEAD existe et est vide
  if (strlen(hash) == 0) {
    free(hash);
    printf("le fichier HEAD est vide");
    return;
  }
  // appel à la fonction createUpdateRef
  // qui écrit hash dans le fichier branch
  createUpdateRef(branch, hash);
  free(hash);
}

// Retourne le contenu de .currentBranch
// c'est à dire renvoie la branche actuelle
char *getCurrentBranch() {
  // Alloc
  FILE *f = fopen(".current_branch", "r"); // Ouverture de .current_branch
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier .current_branch\n");
    return NULL;
  }
  // Lecture dans le fichier afin d'obtenir le nom de la branche
  char buffer[256];
  fgets(buffer, 256, f);
  char *s = strdup(buffer);
  fclose(f);
  return s;
}

// affiche la branch donnée en paramètre
void printBranch(char *branch) {
  if (branchExists(branch) == 0) {
    printf("La branche %s n'existe pas\n", branch);
    return;
  }
  char *hash_commit = getRef(branch);
  char *chemin_commit;
  char *precedent; // hash du commit précédent
  char *message; // message associé au commit
  char buffer[256];
  while (hash_commit) { // tant que le hash du commit précédent existe
    chemin_commit = hashToPath(hash_commit); // on transforme hash_commit en chemin
    strcpy(buffer, chemin_commit);
    strcat(buffer, ".c"); // on rajoute l'extension .c
    Commit *c = ftc(buffer); // on transforme le fichier en commit
    if (commitGet(c, "predecessor")) {
      precedent = strdup(commitGet(c, "predecessor")); // on obtient le hash du predecessor
    } 
    else {
      precedent = NULL; // sinon on le met à null car il n'existe pas
    }
    message = commitGet(c, "message"); // on obtient le message
    printf("%s\n", hash_commit); // Affichage du hash du commit
    if (message) {
      printf("%s\n\n", message); // Affichage du message s'il existe
    } else {
      printf("\n");
    }
    free(hash_commit);
    hash_commit = precedent; // on met hash_commit à précedent
    libererCommit(c);
    free(chemin_commit);
  }
}


// Retourne la liste des hash des commits d'une branche
List *branchList(char *branch) {
  // Alloc
  if (branchExists(branch) == 0) {
    printf("La branche %s n'existe pas\n", branch);
    return NULL;
  }
  char *hash_commit = getRef(branch); // on obtient le premier commit
  List *l = initList();
  char *chemin_commit;
  char *precedent; // hash du commit précedent
  Cell *cell;
  char buffer[256];

  while (hash_commit) { // tant que la hash du précédent n'est pas null
    chemin_commit = hashToPath(hash_commit); // On transforme le hash en chemin
    strcpy(buffer, chemin_commit);
    strcat(buffer, ".c"); // on rajoute l'extension .c au chemin
    Commit *c = ftc(buffer); // on transforme le fichier en commit
    if (commitGet(c, "predecessor")) {
      precedent = strdup(commitGet(c, "predecessor")); // on obtient le hash du précédent commit
    } else {
      precedent = NULL; // s'il n'existe pas, on met le pointeur à NULL
    }
    cell = buildCell(hash_commit); // on créer une cellule qui contient le hash
    insertFirst(l, cell);  // et on l'ajoute à la liste
    free(hash_commit);
    free(chemin_commit);
    libererCommit(c);
    hash_commit = precedent; // On passe au précédent
  }
  return l; // on retourne la liste
}

// Renvoie la liste des différents hash de tous les commits contenu dans .ref
List *getAllCommits() {
  // Alloc
  List *l = initList();
  List *liste_refs = listdir(".refs"); // On obtient la liste les références
  Cell *c = *liste_refs; // On crée une cellule qui pointe vers la tête de la liste
  List *branch;
  Cell *cell_branch;
  Cell *n_cell;
  while (c) { // on parcours la liste de toutes les branches
    branch = branchList(c->data); // liste des commit de la branche actuelle
    cell_branch = *branch; 
    while (cell_branch) { // on parcours tous les commits de la branche
      if (searchList(l, cell_branch->data) == NULL) { // on teste s'il est déjà dans la liste
        n_cell = buildCell(cell_branch->data); // on l'ajoute s'il n'y est pas
        insertFirst(l, n_cell);
      }
      cell_branch = cell_branch->next; // On passe au suivant
    }
    libererList(branch);
    c = c->next;
  }
  libererList(liste_refs);
  return l;
}


//Restaure le WorkTree associé à un commit
void restoreCommit(char *hash_commit) {
  char buffer[256];
  char *chemin_commit = hashToPath(hash_commit); // on obtient le chemin de l'enregistrement instantané associé au commit
  strcpy(buffer, chemin_commit);
  strcat(buffer, ".c"); // on rajoute l'extension .c au chemin de l'enregistrement instantané
  free(chemin_commit);
  Commit *c = ftc(buffer); // on transforme le fichier en commit
  if (c == NULL) { // si le commit est NULL alors on retourne une erreur
    printf("Erreur lors de la restoration du commit\n");
    return;
  }
  char *hash_worktree = commitGet(c, "tree"); // on obtient le hash du WorkTree associé au commit
  char *chemin_worktree = hashToPath(hash_worktree); // on le transforme en chemin
  strcpy(buffer, chemin_worktree);
  strcat(buffer, ".t"); // on rajoute l'extension .t au chemin du WorkTree
  free(chemin_worktree);
  libererCommit(c);
  WorkTree *wt = ftwt(buffer); // on transforme l'enregistrement instantané en WorkTree
  if (wt == NULL) { // si le WorkTree est NULL on retourne une erreur
    printf("Erreur lors de la restoration du commit\n");
    return;
  }
  restoreWorkTree(wt, "."); // On fait appel à restoreWorkTree avec le répertoire courant pour pouvoir restorer le WorkTree
  libererWorkTree(wt);
}

//Permet de se déplacer dans les branches selon le nom de la branche donné en paramètre
void myGitCheckoutBranch(char *branch) {
  if (branchExists(branch) == 0) { // on regarde d'abord si branche existe
    printf("La branche %s n'existe pas\n", branch);
    return;
  }
  FILE *f = fopen(".current_branch", "w"); // Si elle existe on met à jour le fichier qui contient la branche actuelle (.current_branch)
  if (f == NULL) { // on affiche un message d'erreur si l'ouvertur du fichier .current_branch echoue
    printf("Erreur lors de l'ouverture du fichier .current_branch\n");
    return;
  }
  fprintf(f, "%s", branch); // on écrit le nom de la branche dans .current_branch
  fclose(f);
  char *hash = getRef(branch); // on obtient la référence contenu dans la branche à l'aide de getRef
  if (hash == NULL) {// si ça on echoue on renvoie un message d'erreur
    printf("Erreur de checkout de la branch %s\n", branch);
    return;
  }
  createUpdateRef("HEAD", hash); // Sinon on met à jour la référence de HEAD pour qu'il pointe vers le dernier commit de la branche
  restoreCommit(hash); // Et on fini par restorer le dernier commmit associé à la branche à l'aide de restoreCommit
  free(hash);
}

// retourne une sous-liste dont les éléments correspondent au pattern
// c'est à dire que les n-premières lettres de pattern sont identiques à celles des éléments contenus dans la liste
List *filterList(List *L, char *pattern) {
  // Alloc
  List *list = initList(); // on créer un nouvelle liste
  Cell *cell = *L;
  if (pattern == NULL) { // pattern null, aucun filtre n'est effectué, on retourne la liste vide 
    return list;
  }
  int len = strlen(pattern); // on regarde la longeur du pattern pour pouvoir faire la comparaison
  while (cell) { // on parcours la liste L
    if (strncmp(cell->data, pattern, len) == 0) { // on fait une comparaison qui restreint à la longueur du pattern
      insertFirst(list, buildCell(cell->data)); // si le pattern correspond on l'ajoute à la liste, sinon on ne fait rien
    }
    cell = cell->next; // on passe à l'élément suivant
  }
  return list; // on renvoie la la nouvelle liste
}


// Permet de chercher un commit à partir de son hash
// on effectue une succession de filtre
// pour au final au obtenir un unique hash
void myGitCheckoutCommit(char *pattern) {
  List *allCommits = getAllCommits(); // On obtient la liste des hash de tous les Commits
  List *f_list = filterList(allCommits, pattern); // On fait un premier filtre sur ces hash 
  libererList(allCommits);
  Cell *cell = *f_list; // on créer une cell associé à la liste des hash des commits filtrés
  Cell *n_cell;
  char buffer[256];
  // cas où aucun correspond au pattern
  if (cell == NULL) {
    printf("Erreur : pattern erroné\n"); // aucun hash correspond au pattern
    libererList(f_list);  // dans ce cas, on arrête la fonction et l'utilisateur recommence
    return;
  }
  //cas où il y a un unique élément qui correspond au pattern
  if (cell->next == NULL) { // pour vérifier cela, on regarde si le suivant est null
    createUpdateRef("HEAD", cell->data); // Si c'est la cas, on met à jour la référence de HEAD pour qu'il pointe vers ce Commit
    restoreCommit(cell->data); // on restore alors le Commit associé à ce hash
    libererList(f_list);
    return;
  }
  // autre cas, c'est à dire que la liste contient plusieurs éléments
  int n;
  while (cell->next != NULL) { // il y a donc un élément suivant
    n_cell = cell;
    printf("La liste de hash actuelle :\n");
    while (n_cell) { // on affiche la liste des hash pour que l'utilisateur puisse raffiner son choix
      printf("%s\n", n_cell->data);
      n_cell = n_cell->next;
    }
    printf("Veuillez préciser votre choix :\n");
    n = scanf("%s", buffer); // on demande à l'utilisateur d'affiner son pattern
    if (n == 0){  // Si l'utilisateur n'a pas rentré de valeur
      buffer[0] = '\0'; 
    }
    allCommits = filterList(f_list, buffer); // puis on effectue un filtre sur la liste pour la raffiner
    libererList(f_list);
    f_list = allCommits; // on met à jour le pointeur f_list pour qu'il point vers la liste filtré
    cell = *f_list;
    if (cell == NULL) { // si le nouveau pattern donné ne correspond à rien
      printf("Erreur : pattern erroné\n"); // on quitte directement et l'utilisateur devra recommencer
      libererList(allCommits);
      return;
    }
  }
  // si on sort de la boucle while
  // cela signifie qu'il ne reste plus qu'un unique élément dans la liste
  createUpdateRef("HEAD", cell->data); // et on met à jour la référence de HEAD pour qu'il pointe vers ce Commit
  restoreCommit(cell->data); // et on fini par restorer le Commit associé
  libererList(allCommits);
}


