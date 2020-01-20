#include "Elemento.h"

int readElemento(FILE *fp_in, Elemento *element)
{
    return fscanf(fp_in, "%s %d %d %d %d %d %f %d", &element->name, &element->type, &element->entranceDirection,
                  &element->exitDirection, &element->needPrecedentElement, &element->isFinal, &element->value, &element->difficulty) == 8;
}

int element_IsAcrobaticInDirection(Elemento *element, ElementType type)
{
    return element->type == type;
}

int element_IsAcrobatic(Elemento *element)
{
    return element->type == ACROBATIC_BACKWARD || element->type == ACROBATIC_FORWARD;
}

int element_AreCompatible(Elemento *first, Elemento *second)
{
    // Se first è un elemento finale non è compatibile
    if (first->isFinal)
        return 0;
    // Se second ha direzione di ingresso uguale all'uscita di first
    if (first->exitDirection == second->entranceDirection)
        return 0;

    return 1;
}

int element_CanStartDiag(Elemento *element)
{
    if (element->needPrecedentElement)
        return 0;

    if (element->entranceDirection == BACK)
        return 0;

    return 1;
}