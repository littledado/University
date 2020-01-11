#include <stdio.h>
#include <stdlib.h>

#define N 4 // numero di oggetti pietre preziose
#define FILE_NAME "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab07\\Esercizio2\\e2_test_set.txt"

typedef struct
{
    int n_available;
    int weigth;
    int max_rep;
} PietraPreziosa;

typedef struct
{
    int n_tot;
    PietraPreziosa pietre[N];
} Inventario;

typedef struct
{
    int n_pietre;
    int weigth;
    int *pietre;
    int *pietre_occurencies;
} Collana;

// Legge l'inventario tramite file
Inventario *readInventarioFromFile(FILE *fp, int *n_record);
// Alloca la memoria per una collana di n_pietre
Collana *allocaCollana(int n_pietre);
// Restituisce la memoria della collana
void freeCollana(Collana *collana);
// Stampa la composizione della collana
void printCollana(Collana *collana, char *symbolTable);
// Incrementa la quantità in inventario per la pietra specificata
void increaseInventary(Inventario *inventary, int pietraIndex);
// Decrementa la quantità in inventario per la pietra specificata
void decreaseInventary(Inventario *inventary, int pietraIndex);
// Indica se la pietra specificata è disponibile
int pietraIsAvailable(Inventario *inventary, int pietraIndex);
// Indica se la pietra second può seguire la pietra first tenendo conto delle ripetizioni
int pietraCanFollow(int first, int second, int actualRip, int maxRip);
// Wrapper per caloclare soluzione
Collana *calculateCollana(Inventario *inventario);
// Funzione ricorsiva per calcolare la collana
int calculateCollanaRecursive(Inventario *input, Collana *result, Collana *bestSolution, int pos, int k, int count, int prec, int actualRip, int maxRep);

int main(void)
{
    char st_pietre[N] = {'Z', 'R', 'T', 'S'};

    Collana *collana = NULL;
    FILE *fp_in = NULL;
    int n_record;

    // Apro il file
    fp_in = fopen(FILE_NAME, "r");
    // Leggo inventario
    Inventario *inventario = readInventarioFromFile(fp_in, &n_record);

    for (int i = 0; i < n_record; i++)
    {
        printf("Test collana:%d", i + 1);
        // Creo la collana
        collana = calculateCollana(&inventario[i]);
        // Stampo la collana
        printCollana(collana, st_pietre);
        // Libero la memoria
        freeCollana(collana);
    }

    return EXIT_SUCCESS;
}

Inventario *readInventarioFromFile(FILE *fp, int *n_record)
{
    Inventario *result = NULL;
    int max_rep;
    fscanf(fp, "%d", n_record);

    // alloco la memoria
    result = (Inventario *)malloc(*n_record * sizeof(Inventario));

    for (int i = 0; i < *n_record; i++)
    {
        result[i].n_tot = 0;

        fscanf(fp, "%d %d %d %d %d %d %d %d %d", &result[i].pietre[0].n_available, &result[i].pietre[1].n_available, &result[i].pietre[2].n_available, &result[i].pietre[3].n_available,
               &result[i].pietre[0].weigth, &result[i].pietre[1].weigth, &result[i].pietre[2].weigth, &result[i].pietre[3].weigth,
               &max_rep);

        result[i].pietre[0].max_rep = result[i].pietre[1].max_rep = result[i].pietre[2].max_rep = result[i].pietre[3].max_rep = max_rep;

        for (int j = 0; j < 4; j++)
            result[i].n_tot += result[i].pietre[j].n_available;
    }

    return result;
}

Collana *allocaCollana(int n_pietre)
{
    Collana *collana = (Collana *)malloc(sizeof(Collana));

    collana->n_pietre = n_pietre;
    collana->pietre = calloc(n_pietre, sizeof(int));
    collana->pietre_occurencies = calloc(N, sizeof(int));
    collana->weigth = 0;
    return collana;
}

void freeCollana(Collana *collana)
{
    if (collana != NULL)
    {
        free(collana->pietre);
        free(collana->pietre_occurencies);
        free(collana);
    }
}

void printCollana(Collana *collana, char *symbolTable)
{
    printf("\nCollana di lunghezza:\t%d e peso:\t%d", collana->n_pietre, collana->weigth);

    for (int i = 0; i < collana->n_pietre; i++)
    {
        if (i == 0)
            printf("\n[");

        printf("%c", symbolTable[collana->pietre[i]]);

        if (i != collana->n_pietre - 1)
            printf(", ");
        else
            printf("]\n");
    }
}

int pietraCanFollow(int first, int second, int actualRip, int maxRip)
{
    if (first == second && actualRip >= maxRip)
        return 0;

    switch (first)
    {
    case 0:
        if (!(second == 0 || second == 1))
            return 0;
        break;
    case 1:
        if (!(second == 3 || second == 2))
            return 0;
        break;
    case 3:
        if (!(second == 3 || second == 2))
            return 0;
        break;
    case 2:
        if (!(second == 0 || second == 1))
            return 0;
        break;
    }

    return 1;
}

int pietraIsAvailable(Inventario *inventary, int pietraIndex)
{
    return inventary->pietre[pietraIndex].n_available > 0;
}

void decreaseInventary(Inventario *inventary, int pietraIndex)
{
    inventary->pietre[pietraIndex].n_available--;
}
void increaseInventary(Inventario *inventary, int pietraIndex)
{
    inventary->pietre[pietraIndex].n_available++;
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
        count = calculateCollanaRecursive(inventario, solutionTemp, bestSolutionTemp, 0, i, 0, -1, 0, inventario->pietre[0].max_rep);
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

int calculateCollanaRecursive(Inventario *input, Collana *result, Collana *bestSolution, int pos, int k, int count, int prec, int actualRip, int maxRep)
{
    if (pos >= k && result->weigth > bestSolution->weigth)
    {
        for (int i = 0; i < k; i++)
            bestSolution->pietre[i] = result->pietre[i];
        bestSolution->n_pietre = k;
        bestSolution->weigth = result->weigth;

        return 1;
    }

    for (int i = 0; i < N; i++)
    {

        if (!pietraIsAvailable(input, i))
            continue;
        // Se non è il primo ingresso
        if (prec != -1 && !pietraCanFollow(result->pietre[prec], i, actualRip, maxRep))
            continue;
        // Check sul numero di zaffiri
        if (i == 0 && result->pietre_occurencies[0] + 1 > result->pietre_occurencies[3])
            continue;

        // Se devo azzerare actualRep
        if (result->pietre[prec] != i)
            actualRip = 1;
        else
            actualRip++;

        result->pietre[pos] = i;
        // Incremento occorrenze
        result->pietre_occurencies[i]++;
        // Incremento peso
        result->weigth += input->pietre[i].weigth;

        // Decremento la disponibilità
        decreaseInventary(input, i);

        if (calculateCollanaRecursive(input, result, bestSolution, pos + 1, k, count, pos, actualRip, maxRep))
            return 1;

        // Decremento peso
        result->weigth -= input->pietre[i].weigth;
        // Visto che non ho trovato soluzione, re-incremento la disponibilità
        increaseInventary(input, i);
        // Decremento occorrenze
        result->pietre_occurencies[i]--;
    }

    return 0;
}
