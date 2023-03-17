# include <stdio.h>
# include <stdlib.h>
#include <string.h>

int hashFile(char* source, char* dest){
    char buffer[256];
    sprintf(buffer, "cat %s | sha256sum > %s", source, dest);
    return system(buffer);
}

char* sha256file(char* file){
    // Allocation

    //ouverture
    char template[100];
    sprintf(template, "%sXXXXXX", file);
    char fname[100];
    strcpy(fname, template);
    mkstemp(fname);
    
    //ecriture
    hashFile(file, fname);
    char *hash;
    FILE *f = fopen(fname, "r");
    if (f==NULL) printf("Erreur lors de l'ouverture du fichier %s\n", fname);
    char buffer[65];
    fgets(buffer, 65, f);
    hash = strdup(buffer);

    //fermeture
    fclose(f);
    char nom[120];
    sprintf(nom, "rm %s", fname);
    system(nom);
    return hash;
}


int main() {
    char * hash = sha256file("hachage.c");
    if (hash!=NULL){
        printf("%s\n", hash);
        free(hash);
    }
}
