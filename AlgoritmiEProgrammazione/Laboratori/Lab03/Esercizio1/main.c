#include <stdio.h>
#include <stdlib.h>

#define MAX_N 50

typedef struct
{
    int x;
    int y;
    int base;
    int altezza;
} Rectangle;

typedef struct
{
    int n_rows;
    int n_cols;
    int value[MAX_N][MAX_N];
} Matrix;

void openFile(FILE **fp);
Matrix readMatrixFromFile(FILE *fp);
int getAllRectangles(Matrix matrix, Rectangle rectangles[]);
Rectangle getHigherRectangle(Rectangle rectangles[], int rectanglesNumber);
Rectangle getWiderRectangle(Rectangle rectangles[], int rectanglesNumber);
Rectangle getBiggerRectangle(Rectangle rectangles[], int rectanglesNumber);
Rectangle getEmptyRectangle();
void printRectangle(Rectangle rectangle);
Matrix readMatrixFromUser();
void readMatrix(int matrix[MAX_N][MAX_N], int maxDim, int *nr, int *nc);
int riconosciRegione(int matrix[MAX_N][MAX_N], int n_rows, int n_cols, int r_estremo, int c_estremo, int *base, int *altezza);
int main()
{

    FILE *file_in = NULL;
    Matrix matrix;
    Rectangle rectangles[MAX_N * MAX_N], higherRectangler, widerRectangle, biggerRectangle;
    int rectanglesNumber;

    // Apro il file
    openFile(&file_in);
    // Leggo la matrice
    // matrix = readMatrixFromFile(file_in);
    matrix = readMatrixFromUser();
    // Read all rectangles
    rectanglesNumber = getAllRectangles(matrix, rectangles);
    // cerco rettangolo più alto
    higherRectangler = getHigherRectangle(rectangles, rectanglesNumber);
    // cerco rettangolo con base più alta
    widerRectangle = getWiderRectangle(rectangles, rectanglesNumber);
    // cerco rettangolo con area più grande
    biggerRectangle = getBiggerRectangle(rectangles, rectanglesNumber);

    printf("Max altezza: ");
    printRectangle(higherRectangler);
    printf("Max base: ");
    printRectangle(widerRectangle);
    printf("Max area: ");
    printRectangle(biggerRectangle);

    return EXIT_SUCCESS;
}

int getAllRectangles(Matrix matrix, Rectangle rectangles[])
{
    int rectangleNumbers = 0;
    // Loop sulle righe
    for (int i = 0; i < matrix.n_rows; i++)
    {

        for (int j = 0; j < matrix.n_cols; j++)
        {
            Rectangle newRect = getEmptyRectangle();

            if (riconosciRegione(matrix.value, matrix.n_rows, matrix.n_cols, i, j, &newRect.base, &newRect.altezza)){
                newRect.x = i;
                newRect.y = j;

                rectangles[rectangleNumbers++] = newRect;
            }
        }
    }

    return rectangleNumbers;
}

Rectangle getHigherRectangle(Rectangle rectangles[], int rectanglesNumber)
{
    Rectangle bestRectangle = getEmptyRectangle();

    for (int i = 0; i < rectanglesNumber; i++)
    {
        Rectangle rectangle = rectangles[i];

        if (rectangle.altezza > bestRectangle.altezza)
            bestRectangle = rectangle;
    }

    return bestRectangle;
}
Rectangle getWiderRectangle(Rectangle rectangles[], int rectanglesNumber)
{
    Rectangle bestRectangle = getEmptyRectangle();

    for (int i = 0; i < rectanglesNumber; i++)
    {
        Rectangle rectangle = rectangles[i];

        if (rectangle.base > bestRectangle.base)
            bestRectangle = rectangle;
    }

    return bestRectangle;
}
Rectangle getBiggerRectangle(Rectangle rectangles[], int rectanglesNumber)
{
    Rectangle bestRectangle = getEmptyRectangle();

    for (int i = 0; i < rectanglesNumber; i++)
    {
        Rectangle rectangle = rectangles[i];

        if ((rectangle.base * rectangle.altezza) > (bestRectangle.base * bestRectangle.altezza))
            bestRectangle = rectangle;
    }

    return bestRectangle;
}
Rectangle getEmptyRectangle()
{
    Rectangle temp;
    temp.altezza = 0;
    temp.base = 0;
    temp.x = 0;
    temp.y = 0;

    return temp;
}
void printRectangle(Rectangle rectangle)
{
    printf("Estremo sup. SX = <%d,%d>, BASE = %d, ALTEZZA: %d, AREA = %d\n", rectangle.x, rectangle.y, rectangle.base, rectangle.altezza, rectangle.base * rectangle.altezza);
}
void openFile(FILE **fp)
{
    *fp = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab02\\Esercizio1\\matrice", "r");
}
Matrix readMatrixFromFile(FILE *fp)
{
    Matrix matrix;

    fscanf(fp, "%d %d", &matrix.n_rows, &matrix.n_cols);
    for (int i = 0; i < matrix.n_rows; i++)
    {
        for (int j = 0; j < matrix.n_cols; j++)
        {
            fscanf(fp, "%d", &matrix.value[i][j]);
        }
    }

    return matrix;
}

Matrix readMatrixFromUser()
{
    Matrix matrice;

    readMatrix(matrice.value, MAX_N, &matrice.n_rows, &matrice.n_cols);

    return matrice;
}

void readMatrix(int matrix[MAX_N][MAX_N], int maxDim, int *nr, int *nc)
{
    printf("Inserisci il numero di righe (n < %d):\t", maxDim);
    scanf("%d", nr);
    printf("Inserisci il numero di colonne (n < %d):\t", maxDim);
    scanf("%d", nc);

    for (int i = 0; i < *nr && i < maxDim; i++)
    {
        for (int j = 0; j < *nc && j < maxDim; j++)
            scanf("%d", &matrix[i][j]);
    }
}

int riconosciRegione(int matrix[MAX_N][MAX_N], int n_rows, int n_cols, int r_estremo, int c_estremo, int *base, int *altezza)
{
    int casella = matrix[r_estremo][c_estremo];
    int findStartRectangle = 0;
    int returnValue = 0;

    
    if (casella == 1)
    {
        if (!findStartRectangle)
        {
            // Incremento numero di rettangoli trovati
            returnValue = 1;

            for (int riga = r_estremo, fineAltezza = 0; riga < n_rows && !fineAltezza; riga++)
            {
                for (int colonna = c_estremo, fineBase = 0; colonna < n_cols && !fineBase; colonna++)
                {
                    if (matrix[riga][colonna] == 0)
                    {
                        if (colonna == c_estremo)
                        {
                            fineAltezza = 1;
                        }
                        else
                        {
                            fineBase = 1;
                        }
                    }
                    else
                    {
                        // Incremento base
                        if (riga == r_estremo)
                            (*base)++;
                        // Smarco la cella in modo da non riconteggiarla più
                        matrix[riga][colonna] = 0;
                    }
                }

                if (!fineAltezza)
                    (*altezza)++;
            }
        }
    }

    return returnValue;
}