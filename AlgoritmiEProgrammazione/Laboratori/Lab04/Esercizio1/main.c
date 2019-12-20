#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int readIntegerFromUser();
int gcd(int a, int b);
void printResult(int result);
int isEven(int numberToCheck);
void swapMajorAndMinor(int *a, int *b);

int main() {
  int a, b, result;

  printf("Programma per il cacolo del Massimo Comun Divisore di A e B\n");
  printf("A:\n");
  a = readIntegerFromUser();
  printf("B:\n");
  b = readIntegerFromUser();

  result = gcd(a, b);

  printResult(result);

  return EXIT_SUCCESS;
}

int readIntegerFromUser() {
  int number;
  printf("Inserisci intero:\t ");
  scanf("%d", &number);
  return number;
}
void printResult(int number) {
  printf("Il massimo comun divisore e':%d\t", number);
}

int gcd(int a, int b) {

  // condizione di terminazione
  if (a == b || b == 0)
    return a;

  swapMajorAndMinor(&a, &b);

  if (isEven(a)) {
    if (isEven(b)) {
      return 2 * gcd(a / 2, b / 2);
    } else {
      return gcd(a / 2, b);
    }
  } else {
    if (!isEven(b))
      return gcd((a - b) / 2, b);
    else
      return gcd(a, b / 2);
  }
}

void swapMajorAndMinor(int *major, int *minor) {
  int tmp;

  if (*major < *minor) {
    tmp = *major;
    *major = *minor;
    *minor = tmp;
  }
}
int isEven(int numberToCheck) {
  if (numberToCheck % 2 == 0)
    return 1;

  return 0;
}