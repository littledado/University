#include <stdio.h>
#include <stdlib.h>

#define MAX_DIM 30

void readArray(int array[], int dim);
void checkArrayCondition(int array[], int dim);
void countLength(int array[], int result[], int dim);
void printArray(int array[], int index, int dim);
int countMaxLength(int array[], int dim);
int IsNull(int value);
int main() {
  int arrayDimension = 0;
  int array[MAX_DIM];

  printf("-- ESERCIZIO 1 --\n\n");

  printf("Inserire il numero di interi del vettore:\t");
  scanf("%d", &arrayDimension);

  if (arrayDimension > 0) {
    readArray(array, arrayDimension);
    checkArrayCondition(array, arrayDimension);
  } else {
    return EXIT_FAILURE;
  }

  return 0;
}

void readArray(int array[], int dim) {
  printf("Inserisci elementi dell'array:\n");

  for (int i = 0; i < dim; i++) {
    printf("%d)\t", i + 1);
    scanf("%d", &array[i]);
  }
}

void checkArrayCondition(int array[], int dim) {
  int lengthArray[MAX_DIM] = {0};
  int maxLength = 0;

  countLength(array, lengthArray, dim);
  maxLength = countMaxLength(lengthArray, dim);
  for (int i = 0; i < dim; i++) {
    if (lengthArray[i] == maxLength) {
      printArray(array, i, lengthArray[i]);
            printf("\n");
    }
  }
        printf("\n");
}

int IsNull(int value) {
  if (value == 0)
    return 1;

  return 0;
}

void countLength(int array[], int result[], int dim) {
  int actualLen = 0, saved_index = -1;
  for (int i = 0; i < dim; i++) {

    if (IsNull(array[i]) ) {
      if (actualLen == 0) {
        result[i] = actualLen;
        actualLen = 0;
        saved_index = -1;
      } else {
        result[saved_index] = actualLen;
        actualLen = 0;
        saved_index = -1;
      }

    } else {
      if (saved_index == -1)
        saved_index = i;

      actualLen++;
    }
  }

  if (actualLen > 0)
  result[saved_index] = actualLen;
}

int countMaxLength(int array[], int dim) {
  int temp_best = -1;
  for (int i = 0; i < dim; i++) {
    if (array[i] > temp_best)
      temp_best = array[i];
  }
  return temp_best;
}

void printArray(int array[], int index, int dim) {
  for (int i = index; i < index + dim; i++) {
    printf("%d\t", array[i]);
  }
}