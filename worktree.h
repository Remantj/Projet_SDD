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

#endif