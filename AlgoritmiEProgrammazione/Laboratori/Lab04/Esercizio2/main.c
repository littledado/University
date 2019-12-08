#include <stdio.h>
#include <stdlib.h>

typedef struct t_array
{
    int *arrayValue;
    int dim;
} Array;

Array readArray();
void freeArray(Array *array);
int majority(int *a, int N);
int majortiyR(int *a, int n, int l, int r);
int count(int *a, int number, int n);

int main()
{
    Array inputArray;
    int majorityItem;

    // leggo array
    inputArray = readArray();

    if ((majorityItem = majority(inputArray.arrayValue, inputArray.dim)) > 0)
        printf("Elemento maggioritario: %d", majorityItem);
    else
        printf("Nessun elemento maggioritario");

    freeArray(&inputArray);

    return EXIT_SUCCESS;
}

int majority(int *a, int N)
{
    return majortiyR(a, N, 0, N - 1);
}

int majortiyR(int *a, int n, int l, int r)
{
    int majorityRight, majorityLeft;
    int majorityRightCount, majorityLeftCount;
    int m = (l + r) / 2;

    // Condizione di terminazione
    if (l >= r)
        return a[l];

    majorityLeft = majortiyR(a, n, l, m);
    majorityRight = majortiyR(a, n, m + 1, r);

    if (majorityLeft == majorityRight)
        return majorityLeft;

    majorityLeftCount = count(a, majorityLeft, n);
    majorityRightCount = count(a, majorityRight,n);


    if (majorityLeftCount > n / 2)
        return majorityLeft;
    if (majorityRightCount > n / 2)
        return majorityRight;

    return -1;
}

int count(int *a, int number, int n)
{
    int counter = 0;

    for (int i = 0; i < n; i++)
        if (a[i] == number)
            counter++;

    return counter;
}

Array readArray(){
    Array arrayToRead;

    printf("Inserisci dimensione dell'array:\t");
    scanf("%d", &arrayToRead.dim);

    arrayToRead.arrayValue = (int *)malloc(arrayToRead.dim * sizeof(int));

    for(int i = 0; i < arrayToRead.dim; i++){
        printf("Elemento %d: ", i+1);
        scanf("%d", &arrayToRead.arrayValue[i]);
    }

    return arrayToRead;
}

void freeArray(Array *array){
    free(array->arrayValue);
}