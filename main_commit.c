#include "LC.h"
#include "commit.h"
#include "directory.h"
#include "hachage.h"
#include "worktree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  // createKeyVal
  kvp *kv1 = createKeyVal("tree", "hash");
  char *str1 = kvts(kv1);
  printf("kv1 = %s\n", str1);

  // stkv
  char *str2 = (char *)malloc(65 * sizeof(char));
  strcpy(str2, "koicoubeh:apagnan");
  kvp *kv2 = stkv(str2);
  char *str3 = kvts(kv2);
  printf("kv2 = %s\n", str3);

  // initCommit
  Commit *c1 = initCommit();

  // commitSet
  commitSet(c1, "calomnie", "je n'y crois pas");
  commitSet(c1, "Jade", "OrLecé");

  // createCommit
  Commit *c2 = createCommit("hash2");
  commitSet(c2, "Alibaba", "40 filles joyeuses");

  // commitGet
  char *str4 = commitGet(c1, "calomnie");
  char *str8 = commitGet(c1, "Schrudlle");
  printf("commitGet de calomnie = %s\n", str4);
  printf("commitGet de Schrudlle = %s\n", str8);

  // cts
  char *str5 = cts(c1);
  char *str6 = cts(c2);
  printf("commit1 = %s\n", str5);
  printf("commit2 = %s\n", str6);

  // stc
  Commit *c3 = stc(str6);
  char *str7 = cts(c3);
  printf("commit3 = %s\n", str7);

  // ctf
  ctf(c3, "test_commit.txt");

  // ftc
  Commit *c4 = ftc("test_commit.txt");
  char *str10 = cts(c4);
  printf("commit4 = %s\n", str10);

  // blobCommit
  char *str9 = blobCommit(c2);
  printf("Le hash de l'enregistrement instantané de c2 : %s\n", str9);

  // initRefs
  initRefs();

  // createUpdateRef
  createUpdateRef("HEAD", "hash");
  createUpdateRef("master", "hash");
  createUpdateRef("test", "hash2");

  // deleteRef
  deleteRef("test");

  // getRef
  char *str11 = getRef("HEAD");
  printf("Le contenu de HEAD est : %s\n", str11);

  // myGitAdd
  myGitAdd("test_commit.txt");

  // myGitCommit
  myGitCommit("master", "test de myGitCommit");

  free(str1);
  free(str2);
  free(str3);
  free(str5);
  free(str6);
  free(str7);
  free(str9);
  free(str10);
  free(str11);

  freeKeyVal(kv1);
  freeKeyVal(kv2);

  libererCommit(c1);
  libererCommit(c2);
  libererCommit(c3);
  libererCommit(c4);
}
