#include "LC.h"
#include "branch.h"
#include "commit.h"
#include "directory.h"
#include "hachage.h"
#include "worktree.h"
#include "merge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Retourne le WorkTree des éléments qui ne sont pas en conflits et construit la liste des éléments qui le sont
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts) {
  // Alloc
  WorkTree *wt = initWorkTree(); // on créer un nouveau WorkTree
  int i, j;
  int tmp = 1;
  char *name;

  // On parcourt le premier WorkTree et on compare ses éléments avec ceux du deuxième
  // Pour ce faire on parcours également le deuxième
  for (i = 0; i < wt1->n; i++) {
    name = wt1->tab[i].name; // nom de l'éléments du premier WorkTree
    tmp = 1; // indicateur qui au début est à 1
    for (j = 0; j < wt2->n; j++) {
      if ((strcmp(wt2->tab[j].name, name) == 0) &&
          (strcmp(wt2->tab[j].hash, wt1->tab[i].hash))) { // on compare son nom est son hash avec ceux du deuxième WorkTree
        insertFirst(*conflicts, buildCell(name)); // S'ils ont même nom et un hash différent alors on l'ajoute à la liste des conflits
        tmp = 0; // on met l'indicateur tmp à 0
        break; // puis on quitte passe au prochain élément du premier WorkTree
      }
    }
    // Si l'élément n'est pas dans wt2, on l'ajoute au WorkTree final
    if (tmp == 1) {
      appendWorkTree(wt, name, wt1->tab[i].hash, wt1->tab[i].mode);
    }
  }

  // On parcourt le deuxième WorkTree et on compare ses éléments avec ceux du premier
  // Pour ce faire on parcours également le premier
  for (j = 0; j < wt2->n; j++) {
    name = wt2->tab[j].name;
    tmp = 1;
    for (i = 0; i < wt1->n; i++) {
      if ((strcmp(wt1->tab[i].name, name) == 0) &&
          (strcmp(wt2->tab[j].hash, wt1->tab[i].hash))) { // on compare son nom est son hash avec ceux du deuxième WorkTree
        tmp = 0; // cependant ici, on ne fait rien s'il y a conflit, puisqu'on l'a déja ajouté avant
        break;
      }
    }
    // Si l'élément n'est pas dans wt1, on l'ajoute au WorkTree final
    if (tmp == 1) {
      appendWorkTree(wt, name, wt2->tab[j].hash, wt2->tab[j].mode);
    }
  }
  return wt; // on retourne le WorkTree Final
}

// Retourne le WorkTree présent dans le commit dont le hash est contenu dans la branche branch
WorkTree *branch_to_workTree(char *branch) {
  // Alloc

  // On récupère le hash du commit
  char buffer[256];
  char *hash_commit = getRef(branch); // on récupère le contenu de la branche, c'est à dire le hash de son dernier Commit
  if (hash_commit == NULL) {
    return NULL;
  }

  // On construit le chemin associé afin de retrouver le commit
  char *chemin_commit = hashToPath(hash_commit); // transforme le hash en chemin
  strcpy(buffer, chemin_commit);
  strcat(buffer, ".c"); // on rajoute l'extension ".c"
  Commit *c = ftc(buffer); // puis on transforme le fichier en Commit

  // On récupère le hash du WorkTree et construit le chemin associé
  char *hash_worktree = commitGet(c, "tree"); // on récupère le hash du WorkTree associé
  char *chemin_worktree = hashToPath(hash_worktree); // on transforme ce hash en chemin
  strcpy(buffer, chemin_worktree);
  strcat(buffer, ".t"); // on rajoute l'extension ".t"

  // On libère tout
  free(hash_commit);
  free(chemin_commit);
  free(chemin_worktree);
  libererCommit(c);
  
  return ftwt(buffer); // puis on retourne le WorkTree , en transformant le fichier en WorkTree à l'aide de ftwt(File -> WorkTree)
}

// Fusionne la branche courante avec la branche passée en paramètre si aucun conflit n’existe
// Ici on ne fait pas de test sur les fonctions appelées (valeur NULL)
// car cela compliquerait énormément la fonction
List *merge(char *remote_branch, char *message) {
  char *current_branch = getCurrentBranch();

  // On récupère les WorkTree dans les branches associées
  WorkTree *wt_rb = branch_to_workTree(remote_branch); // le WorkTree la branche donné en paramètre
  WorkTree *wt_cb = branch_to_workTree(current_branch); // le WorkTree la branche courante

  // On construit la liste des conflits et on appelle la fonction mergeWorkTrees
  // afin de savoir s'il y a des conflits
  List *l = initList(); // Créer une nouvelle liste, qui va contenir les conflits
  WorkTree *wt = mergeWorkTrees(wt_rb, wt_cb, &l); // WorkTree qui contients les éléments non en conflit

  // S'il y a des conflits
  if (*l != NULL) {
    free(current_branch);
    libererWorkTree(wt_rb);
    libererWorkTree(wt_cb);
    libererWorkTree(wt);
    return l; // on retourne la liste des conflits
  }

  // Sinon
  // On obtient les hash des commits des deux branches
  char *hash_cb = getRef(current_branch); // hash du dernier Commit de la branche courante
  char *hash_rb = getRef(remote_branch); // hash du dernier Commit de la branche donné en paramètre

  // On sauvegarde notre WorkTree
  char *hash_wt = saveWorkTree(wt, ".");

  // On crée un nouveau commit avec les bon champs
  Commit *c = createCommit(hash_wt); // on créer un nouveau commit qui contient les éléments non en conflits
  commitSet(c, "predecessor", hash_cb); // on ses champs predecessor à jour
  commitSet(c, "merged_predecessor", hash_rb); // et son champ merged_predecessor
  commitSet(c, "message", message); // puis le champ message

  // On crée un enregistrement instantané du commit
  char *hash_commit = blobCommit(c);

  // On actualise les références et on restore le WorkTree
  createUpdateRef(current_branch, hash_commit);
  createUpdateRef("HEAD", hash_commit);
  deleteRef(remote_branch);
  restoreWorkTree(wt, ".");

  // On libère tout
  free(current_branch);
  free(hash_cb);
  free(hash_rb);
  free(hash_wt);
  free(hash_commit);
  libererWorkTree(wt_rb);
  libererWorkTree(wt_cb);
  libererWorkTree(wt);
  libererCommit(c);
  libererList(l);

  return NULL; // et on renvoie Null, car il n'y a pas de conflit
}

// Crée et ajoute un commit de suppression sur la branche branch, correspondant à la suppression des éléments de la liste conflicts
void createDeletionCommit(char *branch, List *conflicts, char *message) {
  char *current_branch = getCurrentBranch(); // on obtient la branche courrante

  // On se déplace sur la branche branch
  myGitCheckoutBranch(branch); 
  WorkTree *wt = branch_to_workTree(branch); // on obtient le WorkTree associé au dernier Commit de cette branche
  if (wt == NULL) {// Si cela echoue on quite la fonction
    myGitCheckoutBranch(current_branch);
    free(current_branch);
    return;
  }
    
  // On écrase le fichier .add
  FILE *f = fopen(".add", "w"); // ouverture du fichier ".add" en écriture
  if (f == NULL) { // si erreur on quitte
    libererWorkTree(wt);
    myGitCheckoutBranch(current_branch);
    free(current_branch);
    return;
  }
  fclose(f); // on ferme le fichier

  // On ajoute les fichiers/répertoires du worktree qui ne font pas partie de la liste des conflits
  int i;
  for (i = 0; i < wt->n; i++) {
    if (searchList(conflicts, wt->tab[i].name) == NULL) { // on regarde s'il appartient à la liste des conflits
      myGitAdd(wt->tab[i].name); // s'il n'appartient pas, alors on l'ajoute à la zone de préparation, c'est à dire au fichier ".add"
    }
  }

  // On effectue le commit avec les éléments qui ne sont pas en conflit
  myGitCommit(branch, message); 

  // On se re-déplace sur la branche précédente
  myGitCheckoutBranch(current_branch);
  free(current_branch);
  libererWorkTree(wt);
}


// Permet de gérer les conflits un à un en demandant l'avis de l'utilisateur
void gestion_conflits(List *l, List **l_cb, List **l_rb, char *cb, char *rb) {

  // On parcourt la liste des conflits
  Cell *cell = *l;
  int n;
  while (cell) { // tant qu'il y a des éléments dans la liste
    printf("%s\n\n", cell->data); // on affiche l'éléments en question
    printf("Veuillez choisir le numéro de la branche où vous souhaitez qu'il soit supprimer\n"); // on affiche la question
    printf("1-%s\n", cb); // pour que l'utilisateur puisse faire son choix
    printf("2-%s\n", rb);
    scanf("%d", &n); // on récupère la réponse de l'utilisateur
    if (n == 1) { // si c'est 1
      insertFirst(*l_cb, buildCell(cell->data)); // on l'ajoute à la liste l_cb (branche courante)
      cell = cell->next; // et au passe à l'élément suivant
    } else if (n == 2) { // si c'est 2
      insertFirst(*l_rb, buildCell(cell->data)); // on l'ajoute à la liste l_rb (branche remote/lointaine)
      cell = cell->next; // et au passe à l'élément suivant
    } else { // si sa réponse est autre on affiche un message d'erreur puis on boucle
      printf("Erreur d'insertion\n");
    }
  }
}
