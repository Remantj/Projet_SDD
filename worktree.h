#ifndef _WORKTREE_H_
#define _WORKTREE_H_

// Structure WorkFile
typedef struct {
    char * name ;
    char * hash ;
    int mode ;
} WorkFile ;

//Structure WorkTree
typedef struct {
    WorkFile * tab ;
    int size ;
    int n ;
} WorkTree ;


// Création d'un WorkFile, on alloue la mémoire et initialise le champ nom
WorkFile* createWorkFile(char* name);

// Libération d'un WorkFile
void libererWorkFile(WorkFile* wf);

// Transformation d'un WorkFile en chaîne de caractères
// On suppose que strlen(wf->name + wf->hash + wf->mode + 2) < 256
char* wfts(WorkFile* wf);

// Tranformation d'une chaîne de caractères en un WorkFile
// On suppose que les champs name et hash ne dépassent pas 256 caractères
WorkFile* stwf(char* ch);

// Initialisation d'un WorkTree et le tableau de WorkFile
WorkTree* initWorkTree();

// Libération d'un WorkTree
void libererWorkTree(WorkTree* wkt);

// Fonction qui permet de savoir si un WorkFile est présent dans le tableau de WorkFile du WorkTree wt.
// Retourne son indice dans le tableau ou -1 si il n'y est pas.
int inWorkTree(WorkTree* wt, char* name);

// Ajoute un WorkFile au tableau du WorkTree
// retourne 0 si l'ajout est réussi sinon renvoie -1 quand il echoue
// On suppose que les champs ne valent pas NULL
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);

// Transforme un WorkTree en chaîne de caractères
// On suppose que strlen(name + hash + mode) < 256 pour chaque WorkFile
// plus précisément, renvoie un chaine de caractère contenant tous les WorkFile du tableau sous leur forme de chaines de caractère
// séparé par un retour à la ligne c'est à dire "\n"
char* wtts(WorkTree* wt);

// Transforme une chaîne de caractères représentant un WorkTree en un WorkTree
// on repère tous les WorkFile contenu dans la chaîne de caractère séparés par des "\n"
// puis on les ajoute un à un dans le WorkTree (quand c'est possible)
WorkTree* stwt(char* s);

// Ecris la représenation sous forme de chaîne de caractère d'un WorkTree dans un fichier nommé file
int wttf(WorkTree* wt, char* file);

// Lis un WorkTree depuis un fichier
// Et le transforme en un WorkTree
// Prérequis, il faut que le fichier soit écrit dans le format d'un WorkTree
// C'est à dire qu'elle contient la représentation sous forme de chaîne de caractère d'un WorkFile par ligne
WorkTree* ftwt(char* file);

// Créer l'enregistrement instantané d'un WorkTree
// Pour faire cela on passe par un fichier temporaire
// puis on obtient le hash du fichier temporaire qui contient le WorkTree
// et copie le contenu du fichier temporaire dans l'enregistrement instantané
// qui est un fichier qui a pour nom le hash du WorkTree
char* blobWorkTree(WorkTree* wt);

// Fonction getChmod donné dans l'énoncé
// cette fonction permet de donner les droit d'un fichier ou répertoire
// remarque: la fonction donne les droits sous forme décimale, cela aura son importance dans la suite
int getChmod(const char *path);

// Réalise un enregistrement instantané du WorkTree et de tout son contenu, cette fonction est récursive
// on suppose que l'on met seulement le nom du répertoire sans le slash final
char* saveWorkTree(WorkTree* wt, char* path);

// Modifie les droits d'un fichier/répertoire donné en paramètre
// on lit un mode qui est en décimale (base 10)
// et on le transforme en octale (base 8)
void setMode(int mode, char *path);

// fonction récursive qui permet de restaurer un WorkTree
// on parcours le tableau de WorkFile, que l'on va restaurer un à un
// on différencie deux cas, l'un où il s'agit d'un fichier et l'autre où il s'agit d'un répertoire
void restoreWorkTree(WorkTree* wt, char* path);

#endif
