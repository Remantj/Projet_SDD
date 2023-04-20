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

// Crée un couple clé valeur en dupliquant les deux champs 
kvp *createKeyVal(char *key, char *val);

// Libère un couple clé valeur
void freeKeyVal(kvp *kv);

// Transforme un key_value_pair en chaîne de caractères
// Sous la forme : cle:valeur
char *kvts(kvp *k);

// Lis un key_value_pair dans une chaîne de caractères
// Pour ce faire, on parcours la chaîne str caractères afin de séparer dans un tableau
// les clés et dans un autre les valeurs puis on fait appelle à createKeyVal
// On suppose que les clé et les valeurs ne dépassent pas 256 caractères
// On suppose que str a le bon format
kvp *stkv(char *str);

// Initialisation d'un commit, on affecte les champs du commit
// et on met toutes les cases de la table de hachage à NULL
Commit *initCommit();

// Libération d'un commit, on parcours la table pour libérer chacunes de ses cases
void libererCommit(Commit *c);

// fonction de hachage choisie djb2
unsigned long djb2(char *str);

// Ajoute un nouvel élément à sa place dans la table de hachage
// On utilise le probing linéaire : g(k, i) = h(k) + i
void commitSet(Commit *c, char *key, char *value);

// Création d'un commit avec le renseignement du champs tree
// On utilise pour ce faire la fonction commitSet
Commit *createCommit(char *hash);

// Récupère un élément d'un commit dont la clé est key, ne duplique pas le key_value_pair
// On utilise également le probing linéaire jusqu'à tomber si l'élément de même clé
// ou si l'on dépasse la taille de la table
char *commitGet(Commit *c, char *key);

// Transforme un commit en chaîne de caractères
// Pour ce faire, on parcours la table de hachage et pour chaque couples dans la table
// on le convertie en chaîne de caractères et on le concatène à la chaîne que l'on retourne
char *cts(Commit *c);

// Lis un commit depuis une chaîne de caractères
// Pour ce faire, on parcours la chaîne jusqu'à atteindre la fin d'une chaîne de caractères représentant un couple
// Puis on ajoute ce couple à la table
// On suppose que ch respecte bien la structure d'un key_value_pair
Commit *stc(char *ch);

// Ecris un commit dans un fichier
// On y écrit la chaîne de caractères représentant ce commit
void ctf(Commit *c, char *file);

// Lit un commit depuis un fichier, on parcours les lignes de celui-ci et on crée le couple clé valeur
// associé à la ligne lue et on l'ajoute à un commit
// On suppose ques les lignes du fichier respectent bien la structure d'un key_value_pair et ne dépassent pas 256 caractères
Commit *ftc(char *file);

// Crée un enregistrement instantané d'un commit
// Pour ce faire, on passe par un fichier temporaire, puis on crée le fichier qui contient les informations sur le commit
// fichier dont le nom dépend du hash du commit
char *blobCommit(Commit *c);

// Initialisation des références
// On crée le dossier .refs/ s'il n'existe pas
// On crée également les fichiers master et HEAD s'ils n'existent pas
void initRefs();

// Modifie le contenu d'une référence par hash
// Pour ce faire, on ouvre le fichier ref_name en mode écriture et on y écrit la hash
void createUpdateRef(char *ref_name, char *hash);

// Supprime la référence dont le nom est passé en paramètre
// On commence par la rechercher dans le dossier .refs/ et si on la trouve, on la supprime
void deleteRef(char *ref_name);

// Récupère le hash contenu dans la référence ref_name
// On commence par lire le contenu de la référence, s'il n'y en a pas
// On retourne la chaîne vide, sinon le hash contenu dans le fichier
char *getRef(char *ref_name);

// Ajoute un fichier ou un répertoire au fichier .add
// On commence par charger le WorkTree du contenu du fichier .add, si ce fichier existe
// Sinon on crée un nouveau WorkTree, puis on ajoute dans celui-ci file_or_folder
// et enfin, on écrit dans le fichier .add le contenu de ce WorkTree
void myGitAdd(char *file_or_folder);

// Réalise un commit sur la branche donné en paramètre avec un message donné
// Prérequis, il faut que le répertoire .ref/ soit initialisé
// et que HEAD pointe bien vers le dernier commit de branch_name
// Si c'est le cas, on récupère le WorkTree dans le fichier ".add"
// on l'associe au nouveau Commit que l'on va ajouter dans la branche
void myGitCommit(char *branch_name, char *message);

#endif
