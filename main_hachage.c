#include "hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *hash = sha256file("hachage.c");
  if (hash != NULL) {
    printf("%s\n", hash);
    free(hash);
  }
}
