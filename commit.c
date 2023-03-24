#include "commit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE 5

kvp *createKeyVal(char *key, char *val) {
  // Alloc

  kvp *kv = (kvp *)malloc(sizeof(kvp));
  if (kv == NULL) {
    return NULL;
  }
  kv->key = strdup(key);
  kv->value = strdup(val);
  return kv;
}

void freeKeyVal(kvp *kv) {
  if (kv != NULL) {
    if (kv->key != NULL) {
      free(kv->key);
    }
    if (kv->value != NULL) {
      free(kv->value);
    }
    free(kv);
  }
}

char *kvts(kvp *k) {
  // Alloc

  if (k == NULL)
    return NULL;
  int taille = strlen(k->key) + strlen(k->value) + 2;
  char *chaine = (char *)malloc(taille * sizeof(char));
  if (chaine == NULL) {
    return NULL;
  }
  strcpy(chaine, k->key);
  strcat(chaine, ":");
  strcat(chaine, k->value);
  return chaine;
}

kvp *stkv(char *str) {
  // Alloc

  if (str == NULL)
    return NULL;
  int i = 0;
  int j = 0;
  char key[256];
  char val[256]; // Possible problème de taille
  while (str[i] != ':') {
    key[i] = str[i];
    i++;
  }
  key[i] = '\0';
  i++;
  while (str[i] != '\0') {
    val[j] = str[i];
    j++;
    i++;
  }
  val[j] = '\0';
  kvp *kv = createKeyVal(key, val);
  return kv;
}

Commit *initCommit() {
  // Alloc

  Commit *c = (Commit *)malloc(sizeof(Commit));
  if (c == NULL) {
    return NULL;
  }
  c->n = 0;
  c->size = TAILLE;
  c->T = (kvp **)malloc(sizeof(kvp *) * c->size);
  if (c->T == NULL) {
    free(c);
    return NULL;
  }
  int i;
  for (i = 0; i < c->size; i++) {
    c->T[i] = NULL;
  }
  return c;
}

void libererCommit(Commit *c) {
  if (c != NULL) {
    int i;
    for (i = 0; i < c->size; i++) {
      freeKeyVal(c->T[i]);
    }
    free(c->T);
    free(c);
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

void commitSet(Commit *c, char *key, char *value) {
  if (c == NULL) {
    printf("Insertion impossible\n");
    return;
  }
  int i = 0;
  int cle;
  kvp *kv;
  while (i < c->size) {
    cle = (int)((djb2(key) + i) % c->size);
    if (c->T[cle] == NULL) {
      kv = createKeyVal(key, value);
      if (kv == NULL) {
        printf("Insertion impossible\n");
        return;
      }
      c->T[cle] = kv;
      c->n++;
      return;
    }
    i++;
  }
  printf("Insertion impossible, la table est pleine\n");
}

Commit *createCommit(char *hash) {
  // Alloc

  Commit *c = initCommit();
  if (c == NULL)
    return NULL;
  commitSet(c, "tree", hash);
  return c;
}

char *commitGet(Commit *c, char *key) {
  int i = 0;
  int cle;
  while (i < c->size) {
    cle = (djb2(key) + i) % c->size;
    if (c->T[cle] != NULL) {
      if (strcmp((c->T[cle])->key, key) == 0) {
        return (c->T[cle])->value;
      }
    }
    i++;
  }
  return NULL;
}

char *cts(Commit *c) {
  if (c == NULL)
    return NULL;
  int i;
  int taille = 0;
  for (i = 0; i < c->n; i++) {
    taille = 256; //taille + strlen(c->T[i]->key) + strlen(c->T[i]->value) + 2;
  }
  char *str = (char *)malloc(1024* sizeof(char));
  if (str == NULL)
    return NULL;
  i = 0;
  strcpy(str, "");
  for (i = 0; i < c->size; i++) {
    if(c->T[i] != NULL){
      char *kv = kvts(c->T[i]);
      strcat(str, kv);
      strcat(str, "\n");
      free(kv);
    }
  }
  return str;
}

Commit *stc(char *ch) {
  if (ch == NULL)
    return NULL;
  Commit *c = initCommit();
  if (c == NULL) {
    return NULL;
  }
  kvp *kv;
  char buffer[256];
  int i = 0, j = 0;
  while (ch[i]) {
    if (ch[i] == '\n') {
      buffer[j] = '\0';
      kv = stkv(buffer);
      commitSet(c, kv->key, kv->value);
      freeKeyVal(kv);
      j = 0;
      i++;
    } else {
      buffer[j] = ch[i];
      j++;
      i++;
    }
  }
  return c;
}

void ctf(Commit *c, char *file) {
  FILE *f = fopen(file, "w");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", file);
    return;
  }
  char *s = cts(c);
  fprintf(f, "%s", s);
  free(s);
  fclose(f);
}

Commit *ftc(char *file) {
  Commit *c = initCommit();
  if (c == NULL) {
    return NULL;
  }
  FILE *f = fopen(file, "r");
  if (f == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", file);
    return NULL;
  }
  char buffer[256];
  kvp *kv;
  int i;
  char *res = fgets(buffer, 256, f);
  while (res) {
    for (i = 0; buffer[i] != '\n'; i++) {
    }
    buffer[i] = '\0';
    kv = stkv(buffer);
    commitSet(c, kv->key, kv->value);
    freeKeyVal(kv);
    res = fgets(buffer, 256, f);
  }
  return c;
}

// char* blobCommit(Commit* c){}

int main() {
  // createKeyVal
  kvp *kv1 = createKeyVal("tree", "poop1");
  char *str1 = kvts(kv1);
  printf("kv1 = %s\n", str1);

  // stkv
  char *str2 = (char *)malloc(65 * sizeof(char));
  strcpy(str2, "koicoubeh:popo2");
  kvp *kv2 = stkv(str2);
  char *str3 = kvts(kv2);
  printf("kv2 = %s\n", str3);

  // initCommit
  Commit *c1 = initCommit();

  // commitSet
  commitSet(c1, "calomnie", "je n'y crois pas");
  commitSet(c1, "Jade", "Orlabite");

  // createCommit
  Commit *c2 = createCommit("azané caca number one");
  commitSet(c2, "Alibaba", "48 filles de joies");
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

  
  free(str1);
  free(str2);
  free(str3);
  //free(str4);
  free(str5);
  free(str6);
  free(str7);
  
  freeKeyVal(kv1);
  freeKeyVal(kv2);
  

  libererCommit(c1);
  libererCommit(c2);
  libererCommit(c3);

}
