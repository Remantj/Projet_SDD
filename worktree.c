#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "worktree.h"
#include "hachage.h"
#include "directory.h"

#define TAILLE 10

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
    if (wt->n == 0) {
        char* s = (char*)malloc(1 * sizeof(char));
        if (s==NULL) return NULL;
        s[0] = '\0';
        return s;
    }
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
        libererWorkFile(wf);
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
    //Alloc
    FILE* f = fopen(file, "r");
    if (f==NULL){
        printf("Erreur lors de l'ouverture du fichier %s\n", file);
        return NULL;
    }
    char buffer[256];
    WorkTree* wt = initWorkTree();
    if (wt == NULL) return NULL;
    WorkFile* wf;
    while(fgets(buffer, 256, f)){
        wf = stwf(buffer);
        if (wf == NULL){
            libererWorkTree(wt);
            fclose(f);
            return NULL;
        }
        if (wt->n >= wt->size){
            libererWorkTree(wt);
            libererWorkFile(wf);
            fclose(f);
            return NULL;
        }
        appendWorkTree(wt, wf->name, wf->hash, wf->mode);
        libererWorkFile(wf);
    }
    fclose(f);
    return wt;
}

// Exercice 5
char* blobWorkTree(WorkTree* wt){
    //création du fichier temporaire
    char tmp[100];
    char* filename = (char*)malloc(100*sizeof(char));
    strcpy(filename,"test"); 
    sprintf(tmp, "%sXXXXXX", filename);
    int id=mkstemp(tmp);
    if(id == -1){
        printf("Création du fichier tmp echoué\n");
        return NULL;
    }

    //écriture du contenu dans le fichier tmp
    if(wttf(wt, tmp) == -1){
        printf("échec dans l'écriture\n");
        return NULL;
    }

    //hachage du contenu de tmp
    char* hash = sha256file(tmp);

    //enregistrement du fichier instantané à partir de tmp
    char *chemin = hashToPath(hash);
    char dir[3] = {chemin[0], chemin[1], '\0'};
    if (! file_exists(dir)){
        char template[10];
        sprintf(template, "mkdir %s", dir);
        system(template);
    }
    char extension[3]={'.','t','\0'};
    strcat(chemin,extension);
    cp(tmp,chemin);
    
    //suppression du fichier temporaire
    char nom[120];
    sprintf(nom, "rm %s", tmp);
    system(nom);
    
    // on oublie pas de free bien evidemment
    free(chemin);
    free(filename);
    
    // valeur de retour: hash du fichier tmp
    return hash;
}

// Fonction getChmod donné dans l'énoncé
int getChmod ( const char * path ){
    struct stat ret ;

    if ( stat(path , &ret) == -1){
    return -1;
    }

    return ( ret.st_mode & S_IRUSR ) |( ret.st_mode & S_IWUSR ) |( ret.st_mode & S_IXUSR ) |/*
    owner*/
    ( ret.st_mode & S_IRGRP ) |( ret.st_mode & S_IWGRP ) |( ret.st_mode & S_IXGRP ) |/*
    group*/
    ( ret.st_mode & S_IROTH ) |( ret.st_mode & S_IWOTH ) |( ret.st_mode & S_IXOTH ) ;/*
    other*/
}


char* saveWorkTree(WorkTree* wt, char* path){
    WorkFile* tab = wt->tab;
    int i;
    struct stat* ret = (struct stat*)malloc(sizeof(struct stat));
    for(i=0; i<(wt->n);i++){
        char name[100];
        sprintf(name, "%s%s", path, tab[i].name);
        stat(name, ret);
        // test si c'est un fichier
        if(S_ISREG(ret->st_mode)){
            // le cas où c'est un fichier
            blobFile(name);
            tab[i].mode = getChmod(name);
            // peut être rajouté un if(tab[i].hash) avant de le free.
            free(tab[i].hash);
            tab[i].hash = sha256file(name);
        }
        else{
            // le cas où c'est un répértoire
            WorkTree* newWT = initWorkTree();
            strcat(name, "/");
            List* L = listdir(name);
            Cell* C = *L;
            while(C){
                appendWorkTree(newWT, C->data, "(null)", getChmod(C->data));
                C = C->next;
            }
            char* s = saveWorkTree(newWT, name);
            // peut être rajouté un if(tab[i].hash) avant de le free.
            free(tab[i].hash);
            tab[i].mode = getChmod(name);
            tab[i].hash = blobWorkTree(newWT);
            
            free(s);
            libererWorkTree(newWT);
            libererList(L); 

        }
    }
    free(ret);
    char* hash = blobWorkTree(wt);
    return hash;    
}

void setMode ( int mode , char * path ){
    char buff [100];
    sprintf(buff ,"chmod %d %s", mode ,path);
    system (buff);
}

void restoreWorkTree(WorkTree* wt, char* path){
    WorkFile* tab = wt->tab;
    int i;
    int fic[2] = {0,0};
    for(i=0; i<wt->n; i++){
        fic[0] = 0;
        fic[1] = 0;
        char* chemin = hashToPath(tab[i].hash);

        // Test d'existence de l'enregistrement instantané
        FILE* f = fopen(chemin, "r");
        if(f) fic[0] = 1;
        char buffer[120];
        strcpy(buffer, chemin);
        strcat(buffer, ".t");
        FILE* t = fopen(buffer, "r");
        if(t) fic[1] = 1;

        // Si le fichier n'existe pas
        if((fic[0] == 0) && (fic[1] == 0)){
            printf("L'enregistrement instantané n'existe pas %s\n",chemin);
            return;
        }

        // S'il en existe deux
        if((fic[0] == 1) && (fic[1] == 1)){
            printf("Collision de l'enregistrement instantané %s\n",chemin);
            fclose(f);
            fclose(t);
            return;
        }

        // Dans ce cas on est dans un fichier
        if((fic[0] == 1) && (fic[1] == 0)){
            char template[150];
            sprintf(template, "%s%s", path, tab[i].name);
            cp(chemin, template);
            setMode(tab[i].mode, template);
            fclose(f);
            free(chemin);
        }

       // Dans ce cas on est dans un répértoire
       if((fic[0] == 0) && (fic[1] == 1)){
            WorkTree* NewWT = ftwt(buffer);
            char template[150];
            sprintf(template, "%s%s/", path, tab[i].name);
            restoreWorkTree(NewWT, template);
            fclose(t);
            free(chemin);
            libererWorkTree(NewWT);
        }

     }


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
    appendWorkTree(wt1, "fichier1", "fichier1.txt", 256);
    appendWorkTree(wt1, "fichier2", "fichier2.txt", 777);

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

    //blobWorkTree
    //char* s5 = blobWorkTree(wt4);

    //saveWorkTree
    WorkTree* wt4 = initWorkTree();
    // Pour pouvoir tester cette fonction il faudra créer au préalable
    // un répértoire exercice5 qui contient:
    // fichier1.txt , exercice1 et fichier2.txt
    appendWorkTree(wt4, "fichier2.txt", "test1", 0); // ici les hash et le mode on met
    appendWorkTree(wt4, "exercice1", "test1", 0);   // n'importe quoi car on va les 
    appendWorkTree(wt4, "fichier1.txt", "test1", 0); // modifier par la suite
    char* s6= saveWorkTree(wt4, "exercice5/");
    


    libererWorkFile(wf1);
    libererWorkFile(wf2);
    libererWorkTree(wt1);
    libererWorkTree(wt2);
    libererWorkTree(wt3);
    //libererWorkTree(wt4);
    free(s1);
    free(s2);
    free(s3);
    free(s4);
    //free(s5);
    free(s6);
}



