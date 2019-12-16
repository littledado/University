#include <stdio.h>
#include <stdlib.h>

// Prototypes

// Malloc version with return of pointer
int **malloc2dR(int rows, int columns);
// Malloc version with pointer by reference
void malloc2dP(int ***matrix, int rows, int columns);
// Free memory for 2d array
void free2d(int **matrix, int nrows);
// Read matrix from File
int** readMatrixFromFile(FILE *fP, int *rows, int *columns);

int main()
{
    int **Matrix;
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
    *matrix = mallod2dR(rows, columns);
}

void free2d(int **matrix, int nrows)
{
    // Free every rows
    for (int i = 0; i < nrows; i++)
        free(matrix[i]);
    // Free the initial pointer
    free(matrix);
}

int** readMatrixFromFile(FILE *fp, int *rows, int *columns){
    int **matrix = NULL;

    fscanf(fp,"%d %d", rows, columns);

    // Alloco la matrice V1
    matrix = malloc2dR(*rows, *columns);
    // Alloco la matrice V2
    // matrix = malloc2dP(&matrix, *rows, *columns);

    for (int i = 0; i < *rows; i++){
        for (int j = 0; j < *columns; j++)
        fscanf(fp, "%d", &matrix[i][j]);
    }

    return matrix;
}