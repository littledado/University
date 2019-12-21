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
} Grafo;

// Alloca la memoria del grafo e lo inizializza secondo i parametri dati
Grafo *allocaGrafo(int N, int E);
// Legge il grafo da un file
Grafo *readGrafoFromFile(FILE *fp_in);

int main()
{
    Grafo *grafo = NULL;
    FILE *fp_in = NULL;

    // Apro il file
    fopen_s(&fp_in, FILE_PATH, "r");
    grafo = readGrafoFromFile(fp_in);

    // Chiudo il file
    fclose(fp_in);

    return EXIT_SUCCESS;
}

Grafo *allocaGrafo(int N, int E)
{
    Grafo *grafo = malloc(sizeof(Grafo));
    grafo->E = E;
    grafo->N = N;
    grafo->archi = (Arco *)malloc(E * sizeof(Arco));

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
