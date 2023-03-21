#ifndef _directory_h_
#define _directory_h_
#include "LC.h"

List* listdir(char* root_dir);
int file_exists(char *file);
void cp(char *to, char *from);
char* hashToPath(char* hash);
void blobFile(char* file);

#endif
