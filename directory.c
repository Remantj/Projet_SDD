#include "LC.h"
#include "directory.h"
#include "hachage.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Le tilde n'est pas reconnu car specifique au shell
List* listdir(char* root_dir){
    // Alloc
    List* l = initList();
    Cell *c;
    DIR * dp = opendir(root_dir);
    struct dirent * ep;
    if (dp != NULL){
        while (( ep = readdir(dp)) != NULL )
        {
            if( strcmp(ep->d_name, ".") != 0 ) {
                 if( strcmp(ep->d_name, "..") != 0 ){
                    c = buildCell(ep->d_name);
                    insertFirst(l, c);
                }
            }
        }
    }
    closedir(dp);
    return l;
}

int file_exists(char *file){
    List *l = listdir(".");
    int n = searchList(l, file) ? 1 : 0;
    libererList(l);
    return n;
}

void cp(char *from, char *to){
    // Ouverture
    FILE* f = fopen(from, "r");
    if (f == NULL){
        printf("Inpossible d'ouvrir le fichier %s\n", from);
        return;
    }
    FILE* t = fopen(to, "w");
    if (t == NULL){
        printf("Impossible d'ouvrir le fichier %s\n", to);
        fclose(f);
        return;
    }

    char buffer[256];
    char* res = fgets(buffer, 256, f);
    while(res){
        fprintf(t, "%s", buffer);
        res = fgets(buffer, 256, f);
    }

    fclose(f);
    fclose(t);

}

char* hashToPath(char* hash){
    //Alloc
    char* chemin = (char*) malloc(100*sizeof(char));
    chemin[0] = hash[0];
    chemin[1] = hash[1];
    chemin[2] = '/';
    int i = 2;
    while(hash[i] != '\0'){
        chemin[i+1] = hash[i];
        i++;
    }
    chemin[i+1] = '\0'; 
    return chemin;
}

void blobFile(char* file){
    char *hash = sha256file(file);
    char *chemin = hashToPath(hash);
    //printf("Le chemin : %s\n", chemin);
    char dir[3] = {chemin[0], chemin[1], '\0'};
    if (! file_exists(dir)){
        char template[10];
        sprintf(template, "mkdir %s", dir);
        system(template);
    }
    cp(file, chemin);

    free(hash);
    free(chemin);
}

/*
int main(){
    List* l = listdir(".");
    char* str = ltos(l);
    if (str != NULL){
        printf("%s\n", str);
        free(str);
    }
    printf("Le fichier directory.c existe : %d\n", file_exists("directory.c"));

    cp("directory.c", "test_dir.txt");

    char* chemin = hashToPath("2564789153hjei");
    printf("Chemin du hash : %s\n", chemin);
    free(chemin);

    blobFile("test_dir.txt");
    

    libererList(l);
}

*/

