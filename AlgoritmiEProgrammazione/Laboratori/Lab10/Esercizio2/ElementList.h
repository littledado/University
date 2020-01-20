#ifndef _ELEMENT_LIST_H
#define _ELEMENT_LIST_H
#include <stdio.h>
#include "Elemento.h"

typedef struct elements_s *ElementsList;

ElementsList createElementList(int n_objects);
void freeElementList(ElementsList elementList);
ElementsList elementList_read(FILE *fp_in);
int elementList_add(ElementsList elementList, Elemento element);
int elementList_size(ElementsList elementList);
Elemento *elementList_getByIndex(ElementsList elementList, int index);

#endif