#include <stdio.h>
#include <stdlib.h>
#define N 4 // numero di oggetti pietre preziose
typedef enum
{
    e_zaffiri = 1,
    e_topazi,
    e_rubini,
    e_smeraldi
} pietraPreziosa;

typedef struct
{
    int n_zaffiri;
    int n_topazi;
    int n_rubini;
    int n_smeraldi;
    int n_tot;
    char objectsType[N];
} Inventario;

typedef struct
{
    int n_pietre;
    char *pietre;
} Collana;

Inventario readInventario();
Collana *createCollana(Inventario inventario);
Collana *allocaCollana(int n_pietre);
void printCollana(Collana *collana);
void freeCollana(Collana *collana);
Collana *calculateCollana(Inventario *inventario);
int calculateCollanaRecursive(Inventario *input, Collana *result, Collana *bestSolution, int pos, int k, int count, int prec);
int pietraIsAvailable(char pietra, Inventario *inv);
int pietraCanFollow(char first, char second);
void decreaseInventary(Inventario *inventary, char pietra);
void increaseInventary(Inventario *inventary, char pietra);

int main(void)
{
    Collana *collana = NULL;
    // Leggo inventario
    Inventario inventario = readInventario();
    // Creo la collana
    collana = calculateCollana(&inventario);
    // Stampo la collana
    printCollana(collana);
    // Libero la memoria
    freeCollana(collana);

    return EXIT_SUCCESS;
}

Inventario readInventario()
{
    Inventario result = {0, 0, 0, 0, 0, {'Z', 'T', 'R', 'S'}};

    printf("Inserisci numero di zaffiri:\t");
    scanf("%d", &result.n_zaffiri);
    printf("Inserisci numero di rubini:\t");
    scanf("%d", &result.n_rubini);
    printf("Inserisci numero di smeraldi:\t");
    scanf("%d", &result.n_smeraldi);
    printf("Inserisci numero di topazi:\t");
    scanf("%d", &result.n_topazi);
    result.n_tot = result.n_rubini + result.n_smeraldi + result.n_topazi + result.n_zaffiri;

    return result;
}

Collana *allocaCollana(int n_pietre)
{
    Collana *collana = (Collana *)malloc(sizeof(Collana));

    collana->n_pietre = n_pietre;
    collana->pietre = calloc(n_pietre, sizeof(char));

    return collana;
}

void freeCollana(Collana *collana)
{
    if (collana != NULL)
    {
        free(collana->pietre);
        free(collana);
    }
}

Collana *calculateCollana(Inventario *inventario)
{
    int count = 0;
    Collana *bestSolution = NULL;
    for (int i = inventario->n_tot; i > 0; i--)
    {
        Collana *solutionTemp = allocaCollana(i);
        Collana *bestSolutionTemp = allocaCollana(i);
        // Calcolo ricorsivo
        count = calculateCollanaRecursive(inventario, solutionTemp, bestSolutionTemp, 0, i, 0, -1);
        // dealloco collana temporanea
        freeCollana(solutionTemp);

        // Se ho trovato almeno una soluzione valida
        if (count > 0)
        {
            bestSolution = bestSolutionTemp;
            break;
        }
        else
            freeCollana(bestSolution);
    }

    return bestSolution;
}

int calculateCollanaRecursive(Inventario *input, Collana *result, Collana *bestSolution, int pos, int k, int count, int prec)
{
    if (pos >= k)
    {
        for (int i = 0; i < k; i++)
            bestSolution->pietre[i] = result->pietre[i];
        bestSolution->n_pietre = k;

        return 1;
    }

    for (int i = 0; i < N; i++)
    {

        if (!pietraIsAvailable(input->objectsType[i], input))
            continue;
        // Se non è il primo ingresso
        if (prec != -1 && !pietraCanFollow(result->pietre[prec], input->objectsType[i]))
            continue;

        result->pietre[pos] = input->objectsType[i];

        // Decremento la disponibilità
        decreaseInventary(input, input->objectsType[i]);

        if (calculateCollanaRecursive(input, result, bestSolution, pos + 1, k, count, pos))
            return 1;

        // Visto che non ho trovato soluzione, re-incremento la disponibilità
        increaseInventary(input, input->objectsType[i]);
    }

    return 0;
}

void printCollana(Collana *collana)
{
    printf("\nCollana di lunghezza:\t%d", collana->n_pietre);

    for (int i = 0; i < collana->n_pietre; i++)
    {
        if (i == 0)
            printf("\n[");

        printf("%c", collana->pietre[i]);

        if (i != collana->n_pietre - 1)
            printf(", ");
        else
            printf("]\n");
    }
}

int pietraCanFollow(char first, char second)
{
    switch (first)
    {
    case 'Z':
        if (!(second == 'Z' || second == 'R'))
            return 0;
        break;
    case 'R':
        if (!(second == 'S' || second == 'T'))
            return 0;
        break;
    case 'S':
        if (!(second == 'S' || second == 'T'))
            return 0;
        break;
    case 'T':
        if (!(second == 'Z' || second == 'R'))
            return 0;
        break;
    }
}

int pietraIsAvailable(char pietra, Inventario *inv)
{
    if (pietra == 'R')
        return inv->n_rubini > 0;
    if (pietra == 'Z')
        return inv->n_zaffiri > 0;
    if (pietra == 'S')
        return inv->n_smeraldi > 0;
    if (pietra == 'T')
        return inv->n_topazi > 0;
}

void decreaseInventary(Inventario *inventary, char pietra)
{
    if (pietra == 'R')
        inventary->n_rubini--;
    if (pietra == 'Z')
        inventary->n_zaffiri--;
    if (pietra == 'S')
        inventary->n_smeraldi--;
    if (pietra == 'T')
        inventary->n_topazi--;
}
void increaseInventary(Inventario *inventary, char pietra)
{
    if (pietra == 'R')
        inventary->n_rubini++;
    if (pietra == 'Z')
        inventary->n_zaffiri++;
    if (pietra == 'S')
        inventary->n_smeraldi++;
    if (pietra == 'T')
        inventary->n_topazi++;
}
