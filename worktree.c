#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worktree.h"
#include "hachage.h"

#define TAILLE 3

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
    //Alloc
    WorkTree* wkt = (WorkTree*)malloc(sizeof(WorkTree));
    if(wkt == NULL) return NULL;
    wkt->tab = (WorkFile*)malloc(TAILLE*sizeof(WorkFile));
    wkt->size=TAILLE;
    wkt->n=0;
    return wkt;
}

void libererWorkTree(WorkTree* wkt){
    if(wkt != NULL){
        for(int i=0; i<wkt->n; i++){
            free(wkt->tab[i].name);
            if (wkt->tab[i].hash){
                free(wkt->tab[i].hash);
            }
        }
        free(wkt->tab);
        free(wkt);
    }
}

int inWorkTree(WorkTree* wt, char* name){
    int i;
    for(i=0; i<wt->n; i++){
        if(strcmp((wt->tab[i]).name, name) == 0){
            return i;
        }
    }
    return -1;
}

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode){
    //Alloc
    if ((inWorkTree(wt, name) == -1) && (wt->n < wt->size)){
        int n = wt->n;
        wt->tab[n].name = strdup(name);
        wt->tab[n].hash = strdup(hash);
        wt->tab[n].mode = mode;
        wt->n = wt->n + 1;
        return 0;
    }
    return -1;
}

char* wtts(WorkTree* wt){
    //Alloc
    //On suppose que la taille des chaines de WorkFile est de 256
    char* s = (char*)malloc((257*(wt->n))*sizeof(char));
    if (s==NULL) return NULL;
    int i, j;
    int c=0;
    char* wf;
    for (i=0; i<wt->n; i++){
        wf = wfts(&(wt->tab[i]));
        if (wf == NULL){
            free(s);
            return NULL;
        }
        for (j=0; wf[j]; j++){
            s[c] = wf[j];
            c++;
        }
        free(wf);
        s[c] = '\n';
        c++;
    }
    s[c] = '\0';
    return s;
}

WorkTree* stwt(char* s){
    //Alloc
    WorkTree* wt = initWorkTree();
    if (wt == NULL) return NULL;
    WorkFile* wf;
    char ch[256];
    int c=0;
    int i=0;
    while(s[c] != '\0'){
        while(s[c] && (s[c] != '\n')){
            ch[i] = s[c];
            c++;
            i++;
        }
        ch[i] = '\0';
        i = 0;
        c++;
        printf("%s\n", ch);
        wf = stwf(ch);
        if (wf == NULL){
            libererWorkTree(wt);
            return NULL;
        }
        if (wt->n >= wt->size){
            libererWorkTree(wt);
            libererWorkFile(wf);
            return NULL;
        }
        appendWorkTree(wt, wf->name, wf->hash, wf->mode);
        printf("%s\n",wt->tab[0].name);
        printf("%s\n",wt->tab[0].hash);
        printf("%d\n",wt->tab[0].mode);
        libererWorkFile(wf);
        wt->n = wt->n + 1;
    }
    return wt;
}

int wttf(WorkTree* wt, char* file){
    FILE* f = fopen(file, "w");
    if (f==NULL){
        printf("Erreur lors de l'ouverture du fichier %s\n", file);
        return -1;
    }
    char* s = wtts(wt);
    if (s == NULL){
        fclose(f);
        return -1;
    }
    fprintf(f, "%s", s);
    free(s);
    fclose(f);
    return 0;
}

WorkTree* ftwt(char* file){
    FILE* f = fopen(file, "r");
    if (f==NULL){
        printf("Erreur lors de l'ouverture du fichier %s\n", file);
        return NULL;
    }
    char* s = (char*)malloc(3000*sizeof(char)); //Possible probleme de taille
    fscanf(f, "%s", s);
    WorkTree* wt = stwt(s);
    free(s);
    if (wt == NULL){
        return NULL;
    }
    return wt;
}


int main(){
    //createWorkFile
    WorkFile* wf1 = createWorkFile("test_createWorkFile");
    printf("%s\n", wf1->name);
    
    //wfts
    char *s1 = wfts(wf1);
    printf("%s\n", s1);

    //stwf
    WorkFile* wf2 = stwf(s1);
    printf("%s\n", wf2->name);

    //initWorkTree
    WorkTree* wt1 = initWorkTree();

    //appendWorkTree
    appendWorkTree(wt1, "fichier1", "dubnduebbdf", 256);
    appendWorkTree(wt1, "fichier2", "jeiuuzbfiid", 777);

    //inWorkTree
    int i;
    i = inWorkTree(wt1, "fichier2");
    if (i != -1) printf("Le fichier %s est à la position %d\n", "fichier2", i);
    i = inWorkTree(wt1, "fichier5");
    if (i==-1) printf("Le fichier %s n'est pas dans le WorkTree\n", "fichier5");

    //wtts
    char* s2 = wtts(wt1);
    printf("Le contenu de wt1 est :\n%s", s2);

    //stwt
    WorkTree* wt2 = stwt(s2);
    char* s3 = wtts(wt2);
    printf("Le contenu de wt2 est :\n%s", s3);

    //wttf
    wttf(wt2, "test_worktree.txt");

    //ftwt
    WorkTree* wt3 = ftwt("test_worktree.txt");
    char* s4 = wtts(wt3);
    printf("Le contenu de wt3 est :\n%s", s4);    


    libererWorkFile(wf1);
    libererWorkFile(wf2);
    libererWorkTree(wt1);
    libererWorkTree(wt2);
    libererWorkTree(wt3);
    free(s1);
    free(s2);
    free(s3);
    free(s4);
}
