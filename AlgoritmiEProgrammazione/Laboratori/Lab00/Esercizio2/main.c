#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 21

int conta(char S[MAX_LEN], int n);
void getFileName(char filename[MAX_LEN]);
int getSubstringLen();
int contaVocali(char S[MAX_LEN], int dimension);

int main()
{
    char fileName[MAX_LEN];
    int substringLen = 0;
    FILE *fp = NULL;

    // Open file
    if (0)
    {
        // Read nameFile
        getFileName(fileName);
        fp = fopen(fileName, "r");
    }
    else
    {
        fp = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab00\\Esercizio2\\testo", "r");
    }

    if (fp == NULL)
        return EXIT_FAILURE;
    // Read substring len
    substringLen = getSubstringLen();

    // Execute Algorithm
    doAlgorithm(fp, substringLen);

    // Close file
    fclose(fp);
    return EXIT_SUCCESS;
}

void getFileName(char filename[MAX_LEN])
{
    printf("Inserisci nome file:\t");
    scanf("%s", filename);
}
int getSubstringLen()
{
    int temp = 0;
    printf("Inserisci lunghezza sottostringe in cui cercare:\t");
    scanf("%d", &temp);
    return temp;
}

void doAlgorithm(FILE *fp, int substringLen)
{
    char parola[MAX_LEN];
    int wordsNumber = 0;
    // Read number of words
    fscanf(fp, "%d", &wordsNumber);

    for (int i = 0; i < wordsNumber; i++)
    {
        int counter = 0;
        //read word
        fscanf(fp, "%s", parola);
        // Count
        counter = conta(parola, substringLen);

        printf("Parola:%s\t, conteggio:%d", parola, counter);
    }
}

int conta(char S[MAX_LEN], int n)
{
    int index = 0, counter = 0;

    while (index + n <= strlen(S))
    {
        if (contaVocali(S + index, n) == 2)
            counter++;

            index++;
    }

    return counter;
}

int contaVocali(char S[MAX_LEN], int dimension)
{
    int counter = 0;

    for (int i = 0; i < dimension; i++)
    {
        char c = toupper(S[i]);

        switch (c)
        {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            counter++;
            break;
        default:
            break;
        }
    }

    return counter;
}