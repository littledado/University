#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_DIM 20

typedef struct
{
    int nr;
    int nc;
    int value[MAX_DIM][MAX_DIM];
} Matrix;

void readFileName(char nameFile[MAX_DIM]);
FILE *readFile(char nameFile[MAX_DIM]);
Matrix readMatrixFromFile(FILE *fp);
void doAlgorithm(Matrix value);
int readMatrixDimensionFromUser();
int isCommandValid(int dim, int nr, int nc);
void stampaMatrice(Matrix value);
void findSquaredSubMatrix(Matrix matrix, int dim);

int main()
{
    Matrix value;
    char nameFile[MAX_DIM];
    FILE *fp;

    // Legge nome file
    readFileName(nameFile);

    // Leggo file
    fp = readFile(nameFile);

    // Leggo matrice dal file
    value = readMatrixFromFile(fp);

    // Esegui algoritmo
    doAlgorithm(value);

    // Chiudo file
    fclose(fp);

    return EXIT_SUCCESS;
}
void readFileName(char nameFile[MAX_DIM])
{
    printf("Inserisci il nome del file:\t");
    scanf("%s", nameFile);
}
FILE *readFile(char nameFile[MAX_DIM])
{
    FILE *fp = NULL;
    // return fopen(nameFile, "r");
    fp = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab00\\Esercizio4\\testo", "r");

    return fp;
}

Matrix readMatrixFromFile(FILE *fp)
{
    Matrix result;
    fscanf(fp, "%d%d", &result.nr, &result.nc);

    for (int i = 0; i < result.nr; i++)
    {
        for (int j = 0; j < result.nc; j++)
        {
            fscanf(fp, "%d", &result.value[i][j]);
        }
    }
    return result;
}

void doAlgorithm(Matrix value)
{
    int dim = 0;

 while (isCommandValid((dim = readMatrixDimensionFromUser()), value.nr, value.nc))
    {
        // cerca matrici e stampa
        findSquaredSubMatrix(value, dim);
    }
}

int readMatrixDimensionFromUser()
{
    int dim = INT_MAX;

    printf("Inserisci dimensione sotto-matrice quadrata:\t");
    scanf("%d", &dim);

    return dim;
}

int isCommandValid(int dim, int nr, int nc)
{
    if (dim >= 1 && dim <= min(nr, nc))
        return 1;

    return 0;
}

void stampaMatrice(Matrix value)
{
    printf("MATRICE:\n");
    for (int i = 0; i < value.nr; i++)
    {
        printf("[");
        for (int j = 0; j < value.nc; j++)
        {
            printf("%d", value.value[i][j]);

            if (j != (value.nc - 1))
                printf(",");
        }
        printf("]\n");
    }
    printf("\n");
}

void findSquaredSubMatrix(Matrix matrix, int dim)
{
    Matrix currentMatrix;
    int occurrence = 0;
    // Set dimension
    currentMatrix.nr = currentMatrix.nc = dim;

    for (int i = 0, writed_row = 0; i < matrix.nr; i++)
    {
        int j = 0;

        for (int writed_col = 0; j < matrix.nc && writed_col < dim; j++)
        {
            if (j + dim > matrix.nc)
                break;

            currentMatrix.value[writed_row][writed_col++] = matrix.value[i][j];
        }
        writed_row++;
        if (writed_row == dim)
        {
            stampaMatrice(currentMatrix);
            writed_row = 0;
            j = ++occurrence;
            i = occurrence;
        }
        //         if (i + dim > matrix.nr)
        // {
        //     break;
        // }
    }
}