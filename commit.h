#ifndef _COMMIT_H_
#define _COMMIT_H_

typedef struct key_value_pair {
  char *key;
  char *value;
} kvp;

typedef struct hash_table {
  kvp **T;
  int n;
  int size;
} HashTable;

typedef HashTable Commit;

kvp *createKeyVal(char *key, char *val);
void freeKeyVal(kvp *kv);
char* kvts(kvp* k);
kvp* stkv(char* str);
Commit* initCommit();
void libererCommit(Commit* c);
unsigned long djb2(char *str);
void commitSet(Commit* c, char* key, char* value);
Commit* createCommit(char* hash);
char* commitGet(Commit* c, char* key);
char* cts(Commit* c);
Commit* stc(char* ch);
void ctf(Commit* c, char* file);
char* blobCommit(Commit* c);

#endif
