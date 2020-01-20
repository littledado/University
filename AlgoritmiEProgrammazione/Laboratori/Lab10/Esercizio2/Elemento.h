#ifndef _ELEMENTO_H_
#define _ELEMENTO_H_
#include <stdio.h>
#define MAX_CHAR 100 + 1 //Dimensione della stringa

typedef enum
{
    ACROBATIC_FORWARD,
    ACROBATIC_BACKWARD,
    TRANSITION
} ElementType;

typedef enum
{
    FRONT,
    BACK
} Direction;

typedef struct elemento_s
{
    char name[MAX_CHAR];
    ElementType type;
    Direction entranceDirection;
    Direction exitDirection;
    int needPrecedentElement;
    int isFinal;
    float value;
    int difficulty;
} Elemento;

int readElemento(FILE *fp_in, Elemento *element);
int element_IsAcrobatic(Elemento *element);
int element_IsAcrobaticInDirection(Elemento *element, ElementType type);
int element_AreCompatible(Elemento *first, Elemento *second);
int element_CanStartDiag(Elemento *element);
#endif