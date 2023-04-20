#include "directory.h"
#include "hachage.h"
#include "worktree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
  // createWorkFile
  WorkFile *wf1 = createWorkFile("test_createWorkFile");
  printf("%s\n", wf1->name);

  // wfts
  char *s1 = wfts(wf1);
  printf("%s\n", s1);

  // stwf
  WorkFile *wf2 = stwf(s1);
  printf("%s\n", wf2->name);

  // initWorkTree
  WorkTree *wt1 = initWorkTree();

  // appendWorkTree
  appendWorkTree(wt1, "fichier1.txt", "hash1", 256);
  appendWorkTree(wt1, "fichier2.txt", "hash2", 777);

  // inWorkTree
  int i;
  i = inWorkTree(wt1, "fichier2");
  if (i != -1)
    printf("Le fichier %s est à la position %d\n", "fichier2", i);
  i = inWorkTree(wt1, "fichier5");
  if (i == -1)
    printf("Le fichier %s n'est pas dans le WorkTree\n", "fichier5");

  // wtts
  char *s2 = wtts(wt1);
  printf("Le contenu de wt1 est :\n%s", s2);

  // stwt
  WorkTree *wt2 = stwt(s2);
  char *s3 = wtts(wt2);
  printf("Le contenu de wt2 est :\n%s", s3);

  // wttf
  wttf(wt2, "test_worktree.txt");

  // ftwt
  WorkTree *wt3 = ftwt("test_worktree.txt");
  char *s4 = wtts(wt3);
  printf("Le contenu de wt3 est :\n%s", s4);

  // blobWorkTree
  char *s5 = blobWorkTree(wt3);

  // saveWorkTree
  WorkTree *wt4 = initWorkTree();
  // Pour pouvoir tester cette fonction il faudra au péalable lancer le fichier
  // worktree.sh (bash worktree.sh)
  appendWorkTree(wt4, "dossier","fe000ea88e17fc20ddfe65563804d0e2e85202eb2321a4023dcf13055ee58cbb", 493);
  appendWorkTree(wt4, "fichier1.txt","634b027b1b69e1242d40d53e312b3b4ac7710f55be81f289b549446ef6778bee", 420);
  char *s6 = saveWorkTree(wt4, "repertoire");

  // restoreWorkTree
  // Il faut au préalable avoir lancer le fichier worktree.sh puis avoir lancé
  // saveWorkTree sur ce répertoire, puis il faut mettre en commentaire l'appel à
  // saveWorkTree et free(s6) 
  // Enfin, il faut modifier les
  // fichiers pour pouvoir voir la restoration
 
  //restoreWorkTree(wt4, "repertoire");

  libererWorkFile(wf1);
  libererWorkFile(wf2);
  libererWorkTree(wt1);
  libererWorkTree(wt2);
  libererWorkTree(wt3);
  libererWorkTree(wt4);
  free(s1);
  free(s2);
  free(s3);
  free(s4);
  free(s5);
  free(s6);
}
