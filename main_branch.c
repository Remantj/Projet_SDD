#include "LC.h"
#include "branch.h"
#include "commit.h"
#include "directory.h"
#include "hachage.h"
#include "worktree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  // Il faut lancer le script branch.sh (bash branch.sh)

  // initBranch
  initBranch();

  // branchExists
  int t = branchExists("HEAD");
  printf("La branche HEAD existe : %d\n", t);

  // createBranch
  createBranch("Feature");

  // getCurrentBranch
  char *str1 = getCurrentBranch();
  printf("La branche actuelle est : %s\n", str1);

  // printBranch
  printBranch("master");

  // branchList
  List *l1 = branchList("master");
  char *str2 = ltos(l1);
  printf("La branche master : %s\n\n", str2);

  // getAllCommits
  List *l2 = getAllCommits();
  char *str3 = ltos(l2);
  printf("Toutes les branches : %s\n", str3);

  // restoreCommit
  // Pour le test, il faut d'abord modifier le workTree associé à ce commit
  // avant de le restorer
  // restoreCommit("3083aa95d76bcf4de616bb60c98ce73424de8bb091c9c6da702f46b66f4d4d9a");

  // myGitCheckoutBranch
  myGitCheckoutBranch("master");

  // filterList
  List *l3 = getAllCommits();
  List *l4 = filterList(l3, "");
  char *str4 = ltos(l3);
  char *str5 = ltos(l4);
  printf("l3= %s\n\n", str4);
  printf("l4= %s\n\n", str5);

  // myGitCheckoutCommit
  myGitCheckoutCommit("");

  libererList(l1);
  libererList(l2);
  libererList(l3);
  libererList(l4);

  free(str1);
  free(str2);
  free(str3);
  free(str4);
  free(str5);
}
