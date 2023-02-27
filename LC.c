#include "LC.h"
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

List* initList(){
	//Alloc
    List* l = (List*)malloc(sizeof(List));
    if (l==NULL) return NULL;
    *l = NULL;
    return l;
}


Cell* buildCell(char* ch){
	//Alloc
	Cell *c = (Cell*)malloc(sizeof(Cell));
	if(c==NULL) return NULL;
	c->data = strdup(ch);
	c->next = NULL;
	return c;
}

void libererCell(Cell* c){
	free(c->data);
	free(c);
}

void libererList(List* l){
	Cell* xs = *l;
	Cell* tmp;
	while (xs){
		tmp = xs;
		xs = xs -> next;
		libererCell(tmp);
	}
	free(l);
}

void insertFirst(List *L, Cell* C){
	C->next = *L;
	*L = C;
}

char* ctos(Cell* c){
	//Alloc
	if(c ==NULL) return NULL;
	char* ch = strdup(c->data);
	return ch;
}

char* ltos(List* L){
	//Alloc
	if(*L==NULL) return NULL;
	Cell* tmp = *L;
	char * ch = (char*)malloc(256*sizeof(char)); // Possible probleme de taille
	strcpy(ch, tmp->data);
	tmp = tmp->next;
	while(tmp){
		strcat(ch,"|");
		strcat(ch, (tmp)->data);
		tmp = tmp->next;
	}
	return ch;
}

Cell* listGet(List* L, int i){
	// On suppose que i < len(L)
	Cell * tmp = *L;
	while(i>1){
		if(tmp==NULL) return NULL;
		tmp = tmp->next;
		i--;
	}
	return tmp;
}

Cell* searchList(List* L, char* str){
	Cell * tmp = *L;
	if (tmp == NULL) return NULL;
	while(tmp){
		if (strcmp(str, tmp->data)==0){
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

//Solution très moche pour cette fonction, si t'as mieux je suis preneur
List* stol(char* s){
	//Alloc
	List* L = initList();
	int i_s = 0;
	int i_tmp = 0;
	char* tmp=(char*)malloc(256*sizeof(char)); // Possible probleme
	Cell* c;
	while(s[i_s] != '\0'){
		if ((s[i_s] == '|') && (i_tmp > 0)){
			tmp[i_tmp] = '\0';
			c = buildCell(tmp);
			insertFirst(L, c);
			i_tmp = 0;
		}
		else{
			tmp[i_tmp] = s[i_s];
			i_tmp++;
		}
		i_s++;
	}
	if (i_tmp>0){
		tmp[i_tmp] = '\0';
		c = buildCell(tmp);
		insertFirst(L, c);
	}
	free(tmp);
	return L;
}

void ltof(List* L, char* path){
	// ouvreture en mode écriture
	FILE* f = fopen(path, "w");
	if (f==NULL) printf("Erreur lors de l'ouverture du fichier %s\n", path);
	char *tmp = ltos(L);
	fprintf(f,"%s",tmp);
	// fermeture
	fclose(f);
	free(tmp);
}

List* ftol(char* path){
	//Alloc
	// ouverture en mode lecture
	FILE* f = fopen(path, "r");
	if (f==NULL) printf("Erreur lors de l'ouverture du fichier %s\n", path);
	char buffer[256];
	fgets(buffer, 256, f);
	List* L = stol(buffer);
	//fermeture
	fclose(f);
	return L;
}

int main(){
	char* ch = "caca";
	char* ch2 = "caca2";
	List* l = initList();
	Cell* c = buildCell(ch);
	Cell* c2 = buildCell(ch2);
	insertFirst(l,c);
	insertFirst(l,c2);
	
	// test de ctos
	char* ch1 = ctos(c);
	printf("test de ctos : %s\n",ch1);
	free(ch1);
	
	//test ltos
	char* ch3 = ltos(l);
	printf("test ltos : %s\n",ch3);
	free(ch3);
	
	// test de listGet
	char *ch4 = ctos(listGet(l,2));
	printf("test de listGet : %s\n",ch4);
	free(ch4);
	
	//test de searchList
	Cell* c3 = searchList(l,"caca");
	char *ch8 = ctos(c3);
	printf("test de searchList : %s\n",ch8);
	free(ch8);
	
	//test de stol
	char* ch5 = "je|mange|des|nems";
	List* l2 = stol(ch5);
	char* ch6 = ltos(l2);
	printf("test de stol : %s\n",ch6);
	free(ch6);
	
	//test de ltof
	ltof(l2, "caca.txt");
	
	//test de ftol
	List* l3 = ftol("caca.txt");
	char* ch7 = ltos(l3);
	printf("test de ftol : %s\n",ch7);
	free(ch7);

	libererList(l);
	libererList(l2);
	libererList(l3);
}
