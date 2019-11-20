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

int main()
{

    FILE *file_in = NULL;
    Matrix matrix;
    Rectangle rectangles[MAX_N * MAX_N], higherRectangler, widerRectangle, biggerRectangle;
    int rectanglesNumber;

    // Apro il file
    openFile(&file_in);
    // Leggo la matrice
    matrix = readMatrixFromFile(file_in);
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
            int findStartRectangle = 0;
            int casella = matrix.value[i][j];

            if (casella == 1)
            {
                if (!findStartRectangle)
                {
                    // Get new empty rectangle
                    Rectangle newRectangle = getEmptyRectangle();
                    newRectangle.x = i;
                    newRectangle.y = j;
                    // Incremento numero di rettangoli trovati
                    rectangleNumbers++;

                    for (int riga = i, fineAltezza = 0; riga < matrix.n_rows && !fineAltezza; riga++)
                    {
                        for (int colonna = j, fineBase = 0; colonna < matrix.n_cols && !fineBase; colonna++)
                        {
                            if (matrix.value[riga][colonna] == 0)
                            {
                                if (colonna == j)
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
                                if (riga == i)
                                    newRectangle.base++;
                                // Smarco la cella in modo da non riconteggiarla più
                                matrix.value[riga][colonna] = 0;
                            }
                        }

                        if (!fineAltezza)
                            newRectangle.altezza++;
                    }

                    // Assegno il rettangolo al vettore
                    rectangles[rectangleNumbers - 1] = newRectangle;
                }
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
