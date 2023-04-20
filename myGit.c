#include "LC.h"
#include "branch.h"
#include "commit.h"
#include "directory.h"
#include "hachage.h"
#include "merge.h"
#include "worktree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  // Sans argument
  if (argc == 1) {
    printf("Usage : myGit <param>\n");
    return -1;
  }

  // init
  if (strcmp(argv[1], "init") == 0) {
    initRefs();
    initBranch();
    return 0;
  }

  // list-refs
  if (strcmp(argv[1], "list-refs") == 0) {
    List *l_refs = listdir(".refs"); // On liste les fichiers dans le répertoire .refs/
    char *liste_refs = ltos(l_refs); // On transforme cette liste en chaîne de caractères
    printf("%s\n", liste_refs); // On affiche cette chaîne
    free(liste_refs);
    libererList(l_refs);
    return 0;
  }

  // create-ref
  if (strcmp(argv[1], "create-ref") == 0) {
    if (argc > 3) {
      char *name = argv[2];
      char *hash = argv[3];
      createUpdateRef(name, hash);
    } else {
      printf("Usage : ./myGit create-ref <name> <hash>\n");
      return -1;
    }
    return 0;
  }

  // delete-ref
  if (strcmp(argv[1], "delete-ref") == 0) {
    if (argc > 2) {
      char *name = argv[2];
      deleteRef(name);
    } else {
      printf("Usage : ./myGit delete-ref <name>\n");
      return -1;
    }
    return 0;
  }

  // add
  if (strcmp(argv[1], "add") == 0) {
    if (argc > 2) {
      int i;
      for (i = 2; i < argc; i++) {
        myGitAdd(argv[i]);
      }
    } else {
      printf("Usage : ./myGit add <elem> [<elem2> <elem3> ...]\n");
    }
    return 0;
  }

  // list-add
  if (strcmp(argv[1], "list-add") == 0) {
    char buffer[256];
    FILE *f = fopen(".add", "r");
    if (f == NULL) {
      printf("Erreur lors de l'ouverture du fichier .add\n");
      return -1;
    }
    char *contenu_add = fgets(buffer, 256, f); // On lit la première ligne du fichier .add
    while (contenu_add) { // On parcours le fichier
      printf("%s", contenu_add); // On affiche les lignes
      contenu_add = fgets(buffer, 256, f);
    }
    fclose(f);
    return 0;
  }

  // clear-add
  if (strcmp(argv[1], "clear-add") == 0) {
    FILE *f_add = fopen(".add", "w"); // On ouvre le fichier en mode écriture afin de l'écraser
    if (f_add != NULL)
      fclose(f_add);
    return 0;
  }

  // commit
  if (strcmp(argv[1], "commit") == 0) {
    char *name;
    char *message;

    if (argc == 3) {
      name = argv[2];
      message = NULL;
    }

    else if (argc >= 5) {
      if (strcmp(argv[3], "-m") == 0) {
        name = argv[2];
        message = argv[4];
      } else {
        printf("Usage : ./myGit commit <branch_name> [-m <message>]\n");
        return -1;
      }
    }

    else {
      printf("Usage : ./myGit commit <branch_name> [-m <message>]\n");
      return -1;
    }
    myGitCommit(name, message);
  }

  // get-current-branch
  if (strcmp(argv[1], "get-current-branch") == 0) {
    char *s = getCurrentBranch();
    printf("La branche courante est : %s\n", s);
    free(s);
    return 0;
  }

  // branch
  if (strcmp(argv[1], "branch") == 0) {
    if (argc == 2) {
      printf("Usage : ./myGit branch <branch-name>\n");
      return -1;
    }
    if (branchExists(argv[2])) {
      printf("Erreur : la branche %s existe déjà\n", argv[2]);
      return -1;
    }
    createBranch(argv[2]);
    return 0;
  }

  // branch-print
  if (strcmp(argv[1], "branch-print") == 0) {
    if (argc == 2) {
      printf("Usage : ./myGit branch-print <branch-name>\n");
      return -1;
    }
    printBranch(argv[2]);
    return 0;
  }

  // checkout-branch
  if (strcmp(argv[1], "checkout-branch") == 0) {
    if (argc == 2) {
      printf("Usage : ./myGit checkout-branch <branch-name>\n");
      return -1;
    }
    myGitCheckoutBranch(argv[2]);
    return 0;
  }

  // checkout-commit
  if (strcmp(argv[1], "checkout-commit") == 0) {
    if (argc == 2) {
      printf("Usage : ./myGit checkout-commit <branch-name>\n");
      return -1;
    }
    myGitCheckoutCommit(argv[2]);
    return 0;
  }

  // merge
  if (strcmp(argv[1], "merge") == 0) {
    if (argc < 4) {
      printf("Usage : ./myGit merge <branch> <message>\n");
      return -1;
    } else {
      List *l = merge(argv[2], argv[3]); // On merge les deux branches
      if (l == NULL) { // S'il n'y avait pas de collisions
        printf("La fusion s'est bien passée\n"); // On affiche seulement un message de réussite
        return 0;
      } else { // Sinon
        int n; // Choix de l'utilisateur
        // On le laisse choisir son option
        printf("Veuillez choisir l'option en entrant le numéro correspondant à "
               "l'option:\n\n");
        printf("1-Garder les fichiers de la branche courante\n");
        printf("2-Garder les fichiers de la branche %s\n", argv[2]);
        printf("3-Choisir manuellement");
        scanf("%d", &n); // On récupère son choix
        char *current_branch = getCurrentBranch(); // On récupère la branche courante
        if (n == 1) { // Choix un
          createDeletionCommit(argv[2], l, "suppression"); // On supprime les fichiers en collisions de la branche branch
          merge(argv[2], argv[3]); // On merge
        }
        if (n == 2) { // Choix deux
          createDeletionCommit(current_branch, l, "suppression"); // On supprime les fichiers en collisions de la branche courante
          merge(argv[2], argv[3]); // On merge
        }
        if (n == 3) { // Choix trois
          List *l_cb = initList(); // On crée la liste des éléments à supprimer dans la branche courante
          List *l_rb = initList(); // On crée la liste des éléments à supprimer dans la branche branch
          gestion_conflits(l, &l_cb, &l_rb, current_branch, argv[2]); // On remplie ces listes
          createDeletionCommit(current_branch, l_cb, "suppression"); // On supprime les fichiers en collisions de la branche courante
          createDeletionCommit(argv[2], l_rb, "suppression"); // On supprime les fichiers en collisions de la branche branch
          merge(argv[2], argv[3]); // On merge
          libererList(l_cb);
          libererList(l_rb);
        } else { // Si une autre valeur a été entrée
          printf("Valeur non reconnue\n");
          free(current_branch);
          libererList(l);
          return -1;
        }
        free(current_branch);
        libererList(l);
        return 0;
      }
    }
  }
}
