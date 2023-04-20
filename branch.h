#ifndef _BRANCH_H_
#define _BRANCH_H_
#include "LC.h"

// Initialise le fichier .current_branch et y écrit "master"
void initBranch();

// Teste si la branche nommée branch existe dans le répertoire .refs/ et renvoie 1 si elle existe 0 sinon
int branchExists(char* branch);

// créer une nouvelle branche, qui pointe vers la référence contenue dans HEAD
// il faut que HEAD existe et pointe bien vers un commit
void createBranch(char* branch);

// Retourne le contenu de .currentBranch, c'est à dire renvoie le nom de la branche actuelle
char* getCurrentBranch();

// affiche tous les commist de la branch donnée en paramètre ainsi que les possibles messages
// Pour ce faire, on parcours la liste chaînée qu'est la branche en nous déplaçant à l'aide du champs predecessor
void printBranch(char* branch);

// Retourne la liste des hash des commits d'une branche
// Pour ce faire, on parcours la branche à l'aide du champs predecessor
List* branchList(char* branch);

// Retourne la liste des différents hash de tous les commits contenu dans le répertoire .ref/
// Pour ce faire, On parcours chaque branche puis chaque commit de la branche et l'ajoute à la liste s'il n'y est pas déjà
// La liste est donc sans doublons
List* getAllCommits();

//Restaure le WorkTree associé à un commit
//on récupère l'enregistrement instantanné associé au commit
//puis le hash du WorkTree associé au commit et on le restaure à l'aide de la fonction RestoreWorkTree
void restoreCommit(char* hash_commit);

//Permet de se déplacer dans les branches selon le nom de la branche donné en paramètre
// On vérifie d'abord l'existence de la branche, puis on met à jour le fichier .current_branch
// Et la référence de HEAD pour qu'elle pointe vers le dernier commit de la branche
void myGitCheckoutBranch(char* branch);

// retourne une sous-liste dont les éléments correspondent au pattern
// c'est à dire que les n-premières lettres de pattern sont identiques aux n-premières lettres des éléments contenus dans la liste
List* filterList(List* L, char* pattern);

// Permet de chercher un commit à partir de son hash
// on effectue une succession de filtre à l'aide de la fonction filterList
// pour au final au obtenir un unique hash 
//Cette fonction va faire une boucle qui va demander à l'utilisateur de raffiner ses demandes jusqu'à arriver à un unique hash
void myGitCheckoutCommit(char* pattern);

#endif
