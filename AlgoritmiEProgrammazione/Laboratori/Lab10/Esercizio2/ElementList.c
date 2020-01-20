#include <stdio.h>
#include <stdlib.h>
#include "ElementList.h"
#include "Elemento.h"

struct elements_s
{
    Elemento *elements;
    int n_elements;
    int actual_elements;
};

ElementsList createElementList(int n_objects)
{
    ElementsList list = (ElementsList)malloc(sizeof(*list));

    list->n_elements = n_objects;
    list->actual_elements = 0;
    list->elements = (Elemento *)malloc(n_objects * sizeof(Elemento));
}

void freeElementList(ElementsList elementList)
{
    free(elementList->elements);
    free(elementList);
}

ElementsList elementList_read(FILE *fp_in)
{
    int n_elements = 0;
    ElementsList list = NULL;
    fscanf(fp_in, "%d", &n_elements);

    // inizializzo la strutytura dati
    list = createElementList(n_elements);

    for (int i = 0; i < n_elements; i++)
    {
        Elemento element;

        if (readElemento(fp_in, &element))
            elementList_add(list, element);
    }

    return list;
}

int elementList_add(ElementsList elementList, Elemento element)
{
    if (elementList->actual_elements < elementList->n_elements)
    {
        elementList->elements[elementList->actual_elements++] = element;
        return 1;
    }
    return 0;
}

int elementList_size(ElementsList elementList)
{
    return elementList->actual_elements;
}

Elemento *elementList_getByIndex(ElementsList elementList, int index)
{
    if (index >= 0 && index < elementList->actual_elements)
        return &elementList->elements[index];

    return NULL;
}