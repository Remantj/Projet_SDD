#ifndef _HACHAGE_H_
#define _HACHAGE_H_

// Ecris dans le fichier dest le hash du contenu du fichier source
int hashFile(char *source, char *dest);

// Retourne le hash du fichier passé en paramètre, utilise un fichier temporaire
char *sha256file(char *file);

#endif
