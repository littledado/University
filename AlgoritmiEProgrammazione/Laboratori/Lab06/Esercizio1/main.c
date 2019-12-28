#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab06\\Esercizio1\\grafo.txt"

typedef struct t_arco
{
    int u;
    int v;
} Arco;

typedef struct t_grafo
{
    int N; // Numero di vertici
    int E; // Numero di archi
    Arco *archi;
    int *vertex;
} Grafo;

// Alloca la memoria del grafo e lo inizializza secondo i parametri dati
Grafo *allocaGrafo(int N, int E);
// Legge il grafo da un file
Grafo *readGrafoFromFile(FILE *fp_in);
// Powerset recursive
int powerset_recursive(Grafo *grafo, int *val, int k, int *sol, int j, int pos, int start);
// Wrapper for powerSet
int calculateAllPowerSet(Grafo *grafo);
// Returns 1 if vertex in input is one of the edge
int isVertexInEdge(Arco arco, int vertex);
// Returns 1 if vertex in input is in almost one edge
int checkEdgesAndVertex(Arco *arco, int dim, int vertex);
// Check if vertexCover
int isVertexCover(Grafo *grafo, int *solution, int solDim);
void printVertexCover(int *solution, int dim);

int main()
{
    Grafo *grafo = NULL;
    FILE *fp_in = NULL;

    // Apro il file
    fopen_s(&fp_in, FILE_PATH, "r");
    // Read grafo
    grafo = readGrafoFromFile(fp_in);

    // Chiudo il file
    fclose(fp_in);

    calculateAllPowerSet(grafo);

    return EXIT_SUCCESS;
}

Grafo *allocaGrafo(int N, int E)
{
    Grafo *grafo = malloc(sizeof(Grafo));
    grafo->E = E;
    grafo->N = N;
    grafo->archi = (Arco *)malloc(E * sizeof(Arco));
    grafo->vertex = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
        grafo->vertex[i] = i;

    return grafo;
}

Grafo *readGrafoFromFile(FILE *fp_in)
{
    int N, E;
    Grafo *result = NULL;

    // leggo i parametri del grafo
    fscanf_s(fp_in, "%d%d", &N, &E);

    result = allocaGrafo(N, E);
    for (int i = 0; i < E; i++)
    {
        fscanf_s(fp_in, "%d %d", &result->archi[i].u, &result->archi[i].v);
    }

    return result;
}

int calculateAllPowerSet(Grafo *grafo)
{
    int count = 0;
    int *solution = (int *)malloc(grafo->N * sizeof(int));

    for (int j = 1; j <= grafo->N; j++)
    {
        count += powerset_recursive(grafo, grafo->vertex, grafo->N, solution, j, 0, 0);
    }
    free(solution);

    return count;
}
int powerset_recursive(Grafo *grafo, int *val, int k, int *sol, int j, int pos, int start)
{
    int count = 0, i;

    if (pos == j)
    {
        if (isVertexCover(grafo, sol, j) == 1)
        {
            printVertexCover(sol, j);

            return 1;
        }
    }
    for (i = start; i < k; i++)
    {
        sol[pos] = val[i];
        count += powerset_recursive(grafo, val, k, sol, j, pos + 1, i + 1);
    }
    return count;
}

int isVertexInEdge(Arco arco, int vertex)
{
    if (arco.u == vertex || arco.v == vertex)
        return 1;

    return 0;
}

int checkEdgesAndVertex(Arco *arco, int dim, int vertex)
{
    for (int i = 0; i < dim; i++)
    {
        if (isVertexInEdge(arco[i], vertex) == 1)
            return 1;
    }

    return 0;
}

// Check if vertexCover
int isVertexCover(Grafo *grafo, int *solution, int solDim)
{
    int counter = 0;
    for (int i = 0; i < solDim && i < grafo->N; i++)
    {
        counter += checkEdgesAndVertex(grafo->archi, grafo->E, solution[i]);
        // entrato = 1;
        // // if (solution[i] == 1)
        // // {
        // //     if (checkEdgesAndVertex(grafo->archi, grafo->E, solution[i]) == 0)
        // //         found = 0;
        // // }
    }

    // if (entrato == 1 && found == 1)
    //     return 1;

    if (counter == solDim)
        return 1;

    return 0;
}

void printVertexCover(int *solution, int dim)
{
    printf("(");
    for (int i = 0; i < dim; i++)
    {
        printf("%d", solution[i]);
        if (i != (dim - 1))
            printf(", ");
    }
    printf(")");
}