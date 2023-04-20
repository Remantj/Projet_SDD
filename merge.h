#ifndef _MERGE_H_
#define _MERGE_H_

#include "worktree.h"
#include "LC.h"

// Retourne le WorkTree des éléments qui ne sont pas en conflits et construit la liste des éléments qui le sont
// on considère que deux éléments sont en conflits lorsqu'ils ont le même nom, mais un hash différent
// on doit parcourir les deux WorkTree pour pouvoir vérifier s'il y a conflit, et cela deux fois
// complexité en n² ce qui n'est pas très optimale
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts);

// Retourne le WorkTree présent dans le dernier Commit de la branche dont le hash est contenu dans la branche branch
// on récupère d'aborde le hash du dernier Commit de la branche
// puis le hash du WorkTree associé, et transforme le fichier en WorkTree
// remarque: cette fonction vas nous être utile dans merge, non demandé par l'énoncé
WorkTree *branch_to_workTree(char *branch);

// Fusionne la branche courante avec la branche passée en paramètre si aucun conflit n’existe
// Ici on ne fait pas de test sur les fonctions appelées (valeur NULL)
// car cela compliquerait énormément la fonction
List *merge(char *remote_branch, char *message);

// Crée et ajoute un commit de suppression sur la branche branch, correspondant à la suppression des éléments de la liste conflicts
// C'est à dire qu'on créer un nouveau commit, qui reprend les éléments de l'ancien commit
// tout en retirant les fichiers et répertoire en conflit
void createDeletionCommit(char* branch, List* conflicts, char* message);

// Permet de gérer les conflits un à un en demandant l'avis de l'utilisateur
// l'utilisateur aura le choix de choisir le fichier de la branche dont il supprimer
// et donc garder la version de l'autre branche
void gestion_conflits(List* l, List** l_cb, List** l_rb, char* cb, char* rb);



#endif
