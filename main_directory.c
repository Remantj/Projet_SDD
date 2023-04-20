#include "directory.h"
#include "LC.h"
#include "hachage.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  // listdir
  List *l = listdir(".refs");
  char *str = ltos(l);
  if (str != NULL) {
    printf("%s\n", str);
    free(str);
  }

  // file_exists
  printf("Le fichier directory.c existe : %d\n", file_exists("directory.c"));

  // cp
  cp("directory.c", "test_dir.txt");

  // hashToPath
  char* chemin = hashToPath("2564789153hjei");
  printf("Chemin du hash : %s\n", chemin);
  free(chemin);

  // blobFile
  blobFile("test_dir.txt");


  libererList(l);
  return 0;
}
