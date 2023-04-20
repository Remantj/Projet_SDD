#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hachage.h"



// Ecris dans le fichier dest le hash du contenu du fichier source
int hashFile(char* source, char* dest){
    char buffer[256]; // on initialise un tableau de char de taille 256
    sprintf(buffer, "cat %s | sha256sum > %s", source, dest); // on y écrit les commande dans la chaîne de caractère
    // la commande on applique la fonction sha256sum sur le contenu du fichier source qu'on redirige dans le fichier dest 
    return system(buffer); // appel système de la commande
}

// Retourne le hash du fichier passé en paramètre
char* sha256file(char* file){
    // Alloc

    //ouverture
    char template[100];
    sprintf(template, "%sXXXXXX", file); // on rajoute XXXXXX après le nom du fichier
    char fname[100];
    strcpy(fname, template);
    mkstemp(fname); // mkstemp créer le fichier temporaire en remplaçant XXXXXX par une chaîne de caractère unique
    
    //ecriture
    hashFile(file, fname); //on écrit le hash du fichier file dans le fichier temporaire
    char *hash;
    FILE *f = fopen(fname, "r"); // on ouvre le fichier temporaire
    if (f==NULL) printf("Erreur lors de l'ouverture du fichier %s\n", fname); // si l'ouverture echoue on affiche un message d'erreur et renvoie null
    char buffer[65]; // On va lire des lignes de taille 64 car la taille d'un hash est de 64
    fgets(buffer, 65, f); // on lit la ligne dans le buffer
    hash = strdup(buffer); // on copie le contenu du buffer, c'est à dire le hash

    //fermeture
    fclose(f); // ferme le fichier
    char nom[120];
    sprintf(nom, "rm %s", fname); 
    system(nom); // appel système pour supprimer le fichier temporaire
    return hash; // on retourne le hash du fichier
}
