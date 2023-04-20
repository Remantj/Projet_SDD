#include "LC.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
  
  char* ch = "chaine1";
  char* ch2 = "chaine2";
  
  List* l = initList();
  Cell* c = buildCell(ch);
  Cell* c2 = buildCell(ch2);
  
  insertFirst(l,c);
  insertFirst(l,c2);

  // test de ctos
  char* ch1 = ctos(c);
  printf("test de ctos : %s\n",ch1);
  free(ch1);

  // test de ltos
  char* ch3 = ltos(l);
  printf("test ltos : %s\n",ch3);
  free(ch3);

  // test de listGet
  char *ch4 = ctos(listGet(l,2));
  printf("test de listGet : %s\n",ch4);
  free(ch4);

  //test de searchList
  Cell* c3 = searchList(l,"chaine1");
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
  ltof(l2, "test_LC.txt");

  //test de ftol
  List* l3 = ftol("test_LC.txt");
  char* ch7 = ltos(l3);
  printf("test de ftol : %s\n",ch7);
  free(ch7);

  libererList(l);
  libererList(l2);
  libererList(l3);
}
