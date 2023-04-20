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


// Veuillez lancer le script merge.sh au préalable (bash merge.sh)

int main() {

  // mergeWorkTrees
  List *conflicts = initList();
  WorkTree *wt1 = initWorkTree();
  WorkTree *wt2 = initWorkTree();
  appendWorkTree(wt1, "fichier1", "hash1", 520);
  appendWorkTree(wt1, "fichier2", "hash2", 520);
  appendWorkTree(wt2, "fichier3", "hash3", 520);
  appendWorkTree(wt2, "fichier2", "hash4", 520);
  WorkTree *wt_merg = mergeWorkTrees(wt1, wt2, &conflicts);
  char *str1 = wtts(wt1);
  char *str2 = wtts(wt2);
  char *str3 = wtts(wt_merg);
  char *str4 = ltos(conflicts);

  printf("wt1 = %s\n\n", str1);
  printf("wt2 = %s\n\n", str2);
  printf("wt_merg = %s\n\n", str3);
  printf("conflicts = %s\n\n", str4);

  // merge


  printf("Branche b1:\n");
  printBranch("b1");
  printf("-----------------------------\n");
  printf("Branche master:\n");
  printBranch("master");

  List *l = merge("b1", "fusion");
  if (l == NULL) {
    printf("\nPas de conflit\n\n");
  } else {
    char *str5 = ltos(l);
    printf("\nconflit = %s\n", str5);
    free(str5);
  }

  //createDeletionCommit

  printf("Branche b2:\n");
  printBranch("b2");
  printf("-----------------------------\n");
  printf("Branche master:\n");
  printBranch("master");

  List *l2 = merge("b2", "fusion2");
  if (l2 == NULL) {
    printf("\nPas de conflit\n");
  } else {
    char *str6 = ltos(l2);
    printf("\nconflit = %s\n\n", str6);
    free(str6);
  }
  
  createDeletionCommit("b2", l2, "delete2");

  printf("Branche b2 après la création du commit de suppression :\n");
  printBranch("b2");
  

  if (l != NULL) {
    libererList(l);
  }
  if (l2 != NULL) {
    libererList(l2);
  }

  libererWorkTree(wt1);
  libererWorkTree(wt2);
  libererWorkTree(wt_merg);
  libererList(conflicts);
  free(str1);
  free(str2);
  free(str3);
  free(str4);

  return 0;
}
