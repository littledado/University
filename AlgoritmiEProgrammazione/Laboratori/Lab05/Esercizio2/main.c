#include <stdio.h>
#include <stdlib.h>

// Prototypes

// Data strucutre for scacchiera
typedef struct t_scacchiera
{
    int *whiteCells;
    int *blackCells;
    int nrows;
    int ncolumns;
    int whiteArrayDim;
    int blackArrayDim;
} Scacchiera;

// Malloc version with return of pointer
int **malloc2dR(int rows, int columns);
// Malloc version with pointer by reference
void malloc2dP(int ***matrix, int rows, int columns);
// Free memory for 2d array
void free2d(int **matrix, int nrows);
// Read matrix from File
int **readMatrixFromFile(FILE *fP, int *rows, int *columns);
// Separa le celle
void separa(int **mat, int nr, int nc, Scacchiera *scacchiera);
// Crea la scacchiera data la matrice nrows * ncols
void readScacchiera(Scacchiera *scacchiera, int **matrix, int nrows, int ncols);
// Indica se la cella è bianca o nera
int IsWhiteCell(int coordX, int coordY);
// Alloca la memoria per la scacchiera
void allocaScacchiera(Scacchiera *scacchiera, int nrows, int ncols);
// Stampa la scacchiera
void printScacchiera(Scacchiera *scacchiera);
// Libera la memoria della scacchiera
void freeScacchiera(Scacchiera *scacchiera);

int main()
{
    int **Matrix, rows, columns;
    FILE *fp = NULL;
    Scacchiera scacchiera;

    // open file
    fp = fopen("C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab05\\Esercizio2\\mat.txt", "r");

    // read matrix
    Matrix = readMatrixFromFile(fp, &rows, &columns);
    // read scacchiera
    readScacchiera(&scacchiera, Matrix, rows, columns);
    // print scacchiera
    printScacchiera(&scacchiera);

    free(Matrix);
    freeScacchiera(&scacchiera);

    return EXIT_SUCCESS;
}

int **malloc2dR(int rows, int columns)
{
    int **matrix;

    matrix = (int **)malloc(rows * sizeof(int *));

    for (int i = 0; i < rows; i++)
        matrix[i] = (int *)malloc(columns * sizeof(int));

    return matrix;
}

void malloc2dP(int ***matrix, int rows, int columns)
{
    *matrix = malloc2dR(rows, columns);
}

void free2d(int **matrix, int nrows)
{
    // Free every rows
    for (int i = 0; i < nrows; i++)
        free(matrix[i]);
    // Free the initial pointer
    free(matrix);
}

int **readMatrixFromFile(FILE *fp, int *rows, int *columns)
{
    int **matrix = NULL;

    fscanf(fp, "%d %d", rows, columns);

    // Alloco la matrice V1
    // matrix = malloc2dR(*rows, *columns);
    // Alloco la matrice V2
    malloc2dP(&matrix, *rows, *columns);

    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *columns; j++)
            fscanf(fp, "%d", &matrix[i][j]);
    }

    return matrix;
}

void readScacchiera(Scacchiera *scacchiera, int **matrix, int nrows, int ncols)
{
    int blackIndex = 0, whiteIndex = 0;
    // Alloco la scacchiera
    allocaScacchiera(scacchiera, nrows, ncols);

    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            if (IsWhiteCell(i, j) == 1)
                scacchiera->whiteCells[whiteIndex++] = matrix[i][j];
            else
                scacchiera->blackCells[blackIndex++] = matrix[i][j];
        }
    }
}

void separa(int **mat, int nr, int nc, Scacchiera *scacchiera)
{
    readScacchiera(scacchiera, mat, nr, nc);
}

int IsWhiteCell(int coordX, int coordY)
{
    if (coordX % 2 == 0 && coordY % 2 == 0)
        return 1;
    if (coordX % 2 != 0 && coordY % 2 != 0)
        return 1;
    return 0;
}

void allocaScacchiera(Scacchiera *scacchiera, int nrows, int ncols)
{
    int dim = 0;
    scacchiera->nrows = nrows;
    scacchiera->ncolumns = ncols;

    dim = nrows * ncols;
    if (dim % 2 == 0)
        scacchiera->whiteArrayDim = scacchiera->blackArrayDim = dim / 2;
    else
    {
        scacchiera->whiteArrayDim = dim / 2 + dim % 2;
        scacchiera->blackArrayDim = dim / 2;
    }
    scacchiera->whiteCells = (int *)malloc(scacchiera->whiteArrayDim * sizeof(int));
    scacchiera->blackCells = (int *)malloc(scacchiera->blackArrayDim * sizeof(int));
}

void printScacchiera(Scacchiera *scacchiera)
{
    printf("Valori su celle bianche: \n");
    for (int i = 0; i < scacchiera->whiteArrayDim; i++)
        printf("%d ", scacchiera->whiteCells[i]);
    printf("\nValori su celle nere: \n");
    for (int i = 0; i < scacchiera->blackArrayDim; i++)
        printf("%d ", scacchiera->blackCells[i]);
}

void freeScacchiera(Scacchiera *scacchiera)
{
    free(scacchiera->blackCells);
    free(scacchiera->whiteCells);
}