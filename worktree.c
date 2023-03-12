#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worktree.h"
#include "hachage.h"

#define taille 3

WorkFile* createWorkFile(char* name){
    //Alloc
    WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile));
    if (wf == NULL) return NULL;
    wf -> name = strdup(name);
    wf -> hash = NULL;
    wf -> mode = 0;
    return wf;
}

void libererWorkFile(WorkFile* wf){
    free(wf->name);
    if(wf->hash){
        free(wf->hash);
    }
    free(wf);
}

char* wfts(WorkFile* wf){
    //Alloc
    char *s = (char*)malloc(sizeof(char)*256); //Possible probleme de taille
    if (s== NULL) return NULL;
    sprintf(s, "%s\t%s\t%d", wf->name, wf->hash, wf->mode);
    return s;
}

WorkFile* stwf(char* ch){
    //Alloc
    char name[256];
    char hash[256];
    int mode;
    WorkFile* wf = (WorkFile*)malloc(sizeof(WorkFile));
    if (wf == NULL) return NULL;
    sscanf(ch, "%s\t%s\t%d", name, hash, &mode);
    wf->name = strdup(name);
    wf->hash = strdup(hash);
    wf->mode = mode;
    return wf;
}

WorkTree* initWorkTree(){
    WorkTree wkt = (WorkTree*)malloc(sizeof(WorkTree));
    if(wkt == NULL) return NULL;
    wkt->tab = (WorkFile*)malloc(taille*sizeof(WorkFile));
    wkt->size=taille;
    wkt->n=0;
    return wkt;
}

void libererWorkTree(WorkTree* wkt){
    if(wkt != NULL){
        for(int i=0, i<wkt->n, i++){
            free(wkt->tab[i]);
        }
        free(wkt->tab);
        free(wkt);
    }
}


int main(){
    WorkFile* wf1 = createWorkFile("test_createWorkFile");
    printf("%s\n", wf1->name);
    
    char *s1 = wfts(wf1);
    printf("%s\n", s1);

    WorkFile* wf2 = stwf(s1);
    printf("%s\n", wf2->name);

    libererWorkFile(wf1);
    libererWorkFile(wf2);
    free(s1);
}