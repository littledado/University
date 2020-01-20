#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ElementList.h"
#include "Elemento.h"

#define NAME_FILE "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab10\\Esercizio2\\elementi.txt"
#define DD 11
#define DP 33
#define MAX_ELEMENTS 5
#define DIAG_NUMBERS 3
typedef struct
{
    Elemento **elements;
    int n_elements;
} Diagonale;

typedef struct
{
    Diagonale *diagonali;
    int n_diag;
    int max_diag;
} DiagonalsList;

int diagonal_getDifficulty(Diagonale *diag);
void calculateDiagonalRecursive(ElementsList elements, Diagonale *diag, DiagonalsList *bestDiag, int pos, int k, Elemento *lastElement);
int checkDiagonalIsGood(Diagonale *diag);
int diagonal_getDifficulty(Diagonale *diag);
int diagonal_getValue(Diagonale *diag, int isLastDiag);
DiagonalsList *calculateDiagonal(ElementsList elements);
int main()
{
    FILE *fp_in = NULL;
    ElementsList elements = NULL;
    // Apro il file
    fp_in = fopen(NAME_FILE, "r");
    // Controllo apertura del file
    if (fp_in == NULL)
        return EXIT_FAILURE;
    // Leggo la strutura dati dal file
    elements = elementList_read(fp_in);
    // Chiudo il file
    fclose(fp_in);

    calculateDiagonal(elements);

    return EXIT_SUCCESS;
}
DiagonalsList *calculateDiagonal(ElementsList elements)
{
    DiagonalsList *diagonals = (DiagonalsList *)malloc(sizeof(*diagonals));
    diagonals->max_diag = 1;
    diagonals->n_diag = 0;
    diagonals->diagonali = (Diagonale *)malloc(diagonals->max_diag * sizeof(Diagonale));
    for (int i = 0; i < diagonals->max_diag; i++)
    {
        diagonals->diagonali[i].n_elements = 0;
        diagonals->diagonali[i].elements = (Elemento **)malloc(MAX_ELEMENTS * sizeof(Elemento *));
        for (int j = 0; j < MAX_ELEMENTS; j++)
            (diagonals->diagonali[i]).elements[j] = (Elemento *)malloc(sizeof(Elemento));
    }

    Diagonale *diag = malloc(sizeof(Diagonale));
    diag->n_elements = 0;
    diag->elements = (Elemento **)malloc(MAX_ELEMENTS * sizeof(Elemento *));
    for (int i = 0; i < MAX_ELEMENTS; i++)
        diag->elements[i] = (Elemento *)malloc(sizeof(Elemento));
    calculateDiagonalRecursive(elements, diag, diagonals, 0, 5, NULL);

    return diagonals;
}
void realloc2D(DiagonalsList **pointer)
{
    (*pointer)->max_diag *= 2;
    (*pointer)->diagonali = realloc((*pointer)->diagonali, (*pointer)->max_diag * sizeof(Diagonale));
}
void calculateDiagonalRecursive(ElementsList elements, Diagonale *diag, DiagonalsList *bestDiag, int pos, int k, Elemento *lastElement)
{

    // Condizione di terminazione che abbia riempito il vettore piuttosto che abbia trovato un elemento terminale
    if (pos >= k || (pos > 0 && lastElement->isFinal))
    {
        diag->n_elements = pos;
        if (checkDiagonalIsGood(diag))
        {
            // Controllo enecessità di riallocazione
            if (bestDiag->n_diag == bestDiag->max_diag)
            {
                int olfSize = bestDiag->max_diag;
                // realloc2D(&bestDiag);
                bestDiag->max_diag = 2 * bestDiag->max_diag;
                bestDiag->diagonali = realloc(bestDiag->diagonali, bestDiag->max_diag * sizeof(Diagonale));

                for (int i = olfSize; i < bestDiag->max_diag; i++)
                {
                    bestDiag->diagonali[i].n_elements = 0;
                    bestDiag->diagonali[i].elements = (Elemento **)malloc(MAX_ELEMENTS * sizeof(Elemento *));
                    for (int j = 0; j < MAX_ELEMENTS; j++)
                        (bestDiag->diagonali[i]).elements[j] = (Elemento *)malloc(sizeof(Elemento));
                }
            }
            // Assegno dimensinoe diagonale
            bestDiag->diagonali[bestDiag->n_diag++].n_elements = pos;
            // assegno elementi della diagonale
            for (int i = 0; i < pos; i++)
            {
                bestDiag->diagonali[bestDiag->n_diag - 1].elements[i] = diag->elements[i];
            }
        }

        return;
    }

    for (int i = 0; i < elementList_size(elements); i++)
    {
        Elemento *nextElement = elementList_getByIndex(elements, i);

        // Se mi trovo in prima posizione devo scegliere un elemento frontale
        if ((pos == 0 && element_CanStartDiag(nextElement)) || (pos > 0 && element_AreCompatible(lastElement, nextElement)))
        {
            // Assegno la soluzione
            diag->elements[pos] = nextElement; // Ricorro su pos+1
            calculateDiagonalRecursive(elements, diag, bestDiag, pos + 1, k, nextElement);
        }
        else
            continue;
    }
}

int checkDiagonalIsGood(Diagonale *diag)
{
    // verifico di includere almeno un elemento acrobatico
    int acrob = 0;
    for (int i = 0; i < diag->n_elements; i++)
    {
        if (element_IsAcrobatic(diag->elements[i]))
            acrob++;
    }
    if (acrob == 0)
        return 0;

    // Difficoltà totale della diagonale deve essere minore di DD
    if (diagonal_getDifficulty(diag) > DD)
        return 0;

    return 1;
}

int diagonal_getDifficulty(Diagonale *diag)
{
    int counter = 0;
    for (int i = 0; i < diag->n_elements; i++)
        counter += diag->elements[i]->difficulty;

    return counter;
}

int diagonal_getValue(Diagonale *diag, int isLastDiag)
{
    float value = 0.0;

    for (int i = 0; i < diag->n_elements; i++)
    {
        int multiplier = 1;

        if ((i == (diag->n_elements - 1)) && isLastDiag && diag->elements[i]->value >= 8)
            multiplier = 1.5;

        value += multiplier * diag->elements[i]->value;
    }

    return value;
}