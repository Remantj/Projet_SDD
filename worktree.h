#ifndef _WORKTREE_H_
#define _WORKTREE_H_


typedef struct {
    char * name ;
    char * hash ;
    int mode ;
} WorkFile ;

typedef struct {
    WorkFile * tab ;
    int size ;
    int n ;
} WorkTree ;

WorkFile* createWorkFile(char* name);
void libererWorkFile(WorkFile* wf);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);

WorkTree* initWorkTree();
void libererWorkTree(WorkTree* wkt);
int inWorkTree(WorkTree* wt, char* name);
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);
char* wtts(WorkTree* wt);
WorkTree* stwt(char* s);
int wttf(WorkTree* wt, char* file);
WorkTree* ftwt(char* file);

//Les 3 fonctions de l'exercice 5
char* blobWorkTree(WorkTree* wt);
char* saveWorkTree(WorkTree* wt, char* path);
void restoreWorkTree(WorkTree* wt, char* path);

#endif
