#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 200
#define MAX_COUPLE 30

typedef struct
{
    char key[MAX_CHAR];
    char value[MAX_CHAR];
} DictionaryItem;
typedef struct
{
    DictionaryItem collection[MAX_COUPLE];
    int dim;
} Dictionary;

Dictionary readDictionaryFromFile(FILE *fp);
int searchInDictionary(Dictionary dic, char key[]);

int main()
{
    FILE *fp_testo, *fp_dizionario, *fp_ricodif;
    Dictionary dictionary;

    fp_dizionario = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab01\\Esercizio2\\dizionario.txt", "r");
    fp_testo = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab01\\Esercizio2\\testo.txt", "r");
    fp_ricodif = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab01\\Esercizio2\\ricodificato.txt", "w");

    // read dictionary
    dictionary = readDictionaryFromFile(fp_dizionario);
    // execute problem
    readAndConvertText(fp_testo, dictionary, fp_ricodif);

    fclose(fp_dizionario);
    fclose(fp_testo);
    fclose(fp_ricodif);

    return EXIT_FAILURE;
}

Dictionary readDictionaryFromFile(FILE *fp)
{
    int numCouple = 0;
    Dictionary dictionary;

    // read number of couple
    fscanf(fp, "%d", &dictionary.dim);

    for (int i = 0; i < dictionary.dim; i++)
    {
        fscanf(fp, "%s %s", dictionary.collection[i].value, dictionary.collection[i].key);
    }

    return dictionary;
}

int searchInDictionary(Dictionary dic, char key[])
{
    for (int i = 0; i < dic.dim; i++)
    {
        if (strcmp(dic.collection[i].key, key) == 0)
            return 1;
    }
    return 0;
}

char *getValue(Dictionary dic, char key[])
{
    for (int i = 0; i < dic.dim; i++)
    {
        if (strcmp(dic.collection[i].key, key) == 0)
        {
            char *temp = dic.collection[i].value;
            return temp;
        }
    }
    return NULL;
}

void readAndConvertText(FILE *fp_in, Dictionary dic, FILE *fp_out)
{
    char buffer[MAX_CHAR];
    char *pointerChar, word[MAX_CHAR];

    while (fgets(buffer, MAX_CHAR, fp_in))
    {

        // sscanf(buffer, "%s", word);

        pointerChar = buffer;
        for (int i = 0; i < strlen(buffer) - 1;)
        {
            int increment = 1;
            char *value = NULL;

            if (!isspace(pointerChar[0]))
            {
                sscanf(pointerChar, "%s", word);
                value = getValue(dic, word);
            }

            if (value != NULL)
            {
                increment = strlen(word);
                fprintf(fp_out, "%s", value);
                fprintf(stdout, "%s", value);
            }
            else
            {
                char a = *pointerChar;

                fputc(a, fp_out);
                printf("%c", a);
            }

            // Incremento puntatore del carattere
            pointerChar += increment;
            i += increment;
        }

        fprintf(fp_out, "%c", '\n');
        fprintf(stdout, "%c", '\n');
    }
}