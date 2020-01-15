#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio1\\att.txt"

typedef struct t_activity
{
    int start;
    int end;
    int length;
} Activity;

typedef struct t_Data
{
    Activity *activities;
    int n_activities;
} ActivityWrapper;

typedef struct
{
    ActivityWrapper *activities;
    int lenght;
    int
} SolutionWrapper;
// Legge attività da un file
ActivityWrapper readActivitiesFromFile(char *fileName);
// calcola l'insieme di attività che massimizza la durata totale
void attSel(ActivityWrapper *activities);
// calcola la lunghezza di una singola attività
void calculateActivityLenght(Activity *activityP);
// Restituisce la lunghezza totale delle attività
int getTotalActivitiesLength(Activity *activities, int n_activities);
// Ritorna 1 se le attività sono tutte compatibili
int areActivitiesCompatible(ActivityWrapper *activities, int n_activities);
// Calcolo ricorsivo della soluzione
void calculateActivitiesLengthRecursive(int pos, ActivityWrapper *activities, SolutionWrapper *bestSolution, SolutionWrapper *solution, int *mark, int k);
// Stampa la singola attività
void printActivity(Activity *activity);
// Stampa una soluzione
void printSolution(SolutionWrapper *solution);
int main()
{
    ActivityWrapper activities = readActivitiesFromFile(FILE_NAME);
    // Se ho avuto un errore
    if (activities.n_activities == 0)
        return EXIT_FAILURE;
    // Effettuo algoritmo
    attSel(&activities);

    return EXIT_SUCCESS;
}

ActivityWrapper readActivitiesFromFile(char *fileName)
{

    FILE *fp_in = fopen(fileName, "r");
    ActivityWrapper activities = {0, 0};
    int n_record;

    if (fp_in == NULL)
        return activities;
    // leggo il numero di record
    fscanf(fp_in, "%d", &n_record);
    // Alloco lo spazio corretto per il vettore
    activities.activities = (Activity *)malloc(n_record * sizeof(Activity));
    activities.n_activities = n_record;
    // Inizializzo il vettore secondo il numero di record letto
    for (int i = 0; i < n_record; i++)
    {
        fscanf(fp_in, "%d %d", &activities.activities[i].start, &activities.activities[i].end);
        calculateActivityLenght(&activities.activities[i]);
    }

    return activities;
}

void calculateActivityLenght(Activity *activityP)
{
    activityP->length = activityP->end - activityP->start;
}
void attSel(ActivityWrapper *activities)
{
    SolutionWrapper *bestSolution = NULL, *solution = NULL;
    int *mark = calloc(activities->n_activities, sizeof(int));

    bestSolution = (SolutionWrapper *)malloc(sizeof(SolutionWrapper));
    solution = (SolutionWrapper *)malloc(sizeof(SolutionWrapper));

    bestSolution->lenght = 0;
    solution->lenght = 0;

    bestSolution->activities = (ActivityWrapper *)malloc(sizeof(ActivityWrapper));
    bestSolution->activities->n_activities = activities->n_activities;
    bestSolution->activities->activities = malloc(activities->n_activities * sizeof(Activity));

    solution->activities = (ActivityWrapper *)malloc(sizeof(ActivityWrapper));
    solution->activities->n_activities = activities->n_activities;
    solution->activities->activities = malloc(activities->n_activities * sizeof(Activity));

    for (int i = 1; i < activities->n_activities; i++)
    {
        calculateActivitiesLengthRecursive(0, activities, bestSolution, solution, mark, i);
    }

    printSolution(bestSolution);
}
void calculateActivitiesLengthRecursive(int pos, ActivityWrapper *activities, SolutionWrapper *bestSolution, SolutionWrapper *solution, int *mark, int k)
{
    // Condizione di terminazione
    if (pos >= k)
    {
        // Aggiorno lunghezza totale della soluzione
        solution->lenght = getTotalActivitiesLength(solution->activities->activities, k);

        if (areActivitiesCompatible(solution->activities, k) && bestSolution->lenght < solution->lenght)
        {
            bestSolution->activities = solution->activities;
            bestSolution->lenght = solution->lenght;
            bestSolution->activities->n_activities = k;
        }

        return;
    }

    for (int i = 0; i < activities->n_activities; i++)
    {
        // Se l'elemento non è ancora stato preso
        if (mark[i] == 0)
        {
            mark[i] = 1;
            // Assegno elemento nella soluzione
            solution->activities->activities[pos] = activities->activities[i];
            // Ricorro
            calculateActivitiesLengthRecursive(pos + 1, activities, bestSolution, solution, mark, k);
            // Backtrack
            mark[i] = 0;
        }
    }
}

int areActivitiesCompatible(ActivityWrapper *activities, int n_activities)
{
    for (int i = 0; i < n_activities; i++)
    {
        for (int j = 0; j < n_activities; j++)
        {
            Activity *activityFirst = &activities->activities[i];
            if (i != j)
            {
                Activity *activitySecond = &activities->activities[j];

                if (activityFirst->start > activitySecond->start && activityFirst->start < activitySecond->end)
                    return 0;
                if (activityFirst->end > activitySecond->start && activityFirst->end < activitySecond->end)
                    return 0;
            }
        }
    }

    return 1;
}

int getTotalActivitiesLength(Activity *activities, int n_activities)
{
    int result = 0;

    for (int i = 0; i < n_activities; i++)
        result += activities[i].length;
    return result;
}

void printSolution(SolutionWrapper *solution)
{

    printf("Migliore soluzione trovata con durata massima:\t %d\n", solution->lenght);

    for (int i = 0; i < solution->activities->n_activities; i++)
    {
        printActivity(&solution->activities->activities[i]);

        if (i != solution->activities->n_activities - 1)
            printf(", ");
    }
}

void printActivity(Activity *activity)
{
    printf("(%d,%d)", activity->start, activity->end);
}