#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SEQ_NAME_FILE "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab02\\Esercizio3\\sequenze.txt"
#define TEXT_NAME_FILE "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab02\\Esercizio3\\testo.txt"
#define MAX_SEQ 20
#define MAX_SEQ_LEN 6    // 5 + '/0'
#define MAX_LINE_LEN 201 // 200 + '/0'
#define MAX_WORD_LEN 26  // 25 + '/0'
#define MAX_OCCUR 10

typedef struct _wordPosition
{
    char word[MAX_WORD_LEN];
    int position;
} WordPosition;

typedef struct _sequenceItem
{
    char sequence[MAX_SEQ_LEN];
    int occurencesNumber;
    WordPosition occurences[MAX_OCCUR];
} SequenceItem;

typedef struct _tableSequence
{
    int sequencesNumber;
    SequenceItem sequences[MAX_SEQ];
} Table;

Table readSequenceTableFromFile(FILE *fp);
Table searchOccurencesInText(FILE *fp, Table table);
Table searchOccurencesInWord(char word[], Table sequenceTable, int position);
Table resetTable(Table table);
void printSequenceItem(SequenceItem sequence);
void printTable(Table table);

int main()
{
    FILE *fp_sequences, *fp_text;
    Table sequenceTable;

    // Open files
    fp_sequences = fopen(SEQ_NAME_FILE, "r");
    fp_text = fopen(TEXT_NAME_FILE, "r");

    // Read table from file
    sequenceTable = readSequenceTableFromFile(fp_sequences);
    sequenceTable = resetTable(sequenceTable);
    // Search occurences
    sequenceTable = searchOccurencesInText(fp_text, sequenceTable);

    // Close all files
    fclose(fp_sequences);
    fclose(fp_text);

    // Print result;
    printTable(sequenceTable);

    return EXIT_SUCCESS;
}

Table searchOccurencesInText(FILE *fp, Table table)
{
    char buffer[MAX_LINE_LEN];
    char tempWord[MAX_WORD_LEN];
    int inizioParola = 0, fineParola = 0;
    int index = 0;
    int counterWord = 0;

    while (fgets(buffer, MAX_LINE_LEN, fp) != NULL)
    {
        for (int i = 0; i < strlen(buffer); i++)
        {
            if (isalnum(buffer[i]))
            {
                if (!inizioParola)
                {
                    index = 0;
                    inizioParola = 1;
                }

                tempWord[index++] = toupper(buffer[i]);
            }
            else if (ispunct(buffer[i]) || isspace(buffer[i]))
            {
                fineParola = 1;
            }

            if (inizioParola && fineParola)
            {
                counterWord++;
                // Convert to string
                tempWord[index] = '\0';
                // Search occurences of sequence in the owrd
                table = searchOccurencesInWord(tempWord, table, counterWord);

                inizioParola = fineParola = 0;
            }
            else
                fineParola = 0;
        }
    }

    return table;
}

Table readSequenceTableFromFile(FILE *fp)
{
    Table sequencesTable;

    // Numero di occorrenze
    fscanf(fp, "%d", &sequencesTable.sequencesNumber);

    for (int i = 0; i < sequencesTable.sequencesNumber; i++)
    {
        fscanf(fp, "%s", sequencesTable.sequences[i].sequence);

        for (int j = 0; j < strlen(sequencesTable.sequences[i].sequence); j++)
            sequencesTable.sequences[i].sequence[j] = toupper(sequencesTable.sequences[i].sequence[j]);
    }

    return sequencesTable;
}

Table searchOccurencesInWord(char word[], Table sequenceTable, int position)
{
    for (int i = 0; i < sequenceTable.sequencesNumber; i++)
    {
        if (strstr(word, sequenceTable.sequences[i].sequence))
        {           
            int occ = sequenceTable.sequences[i].occurencesNumber;

            if (occ < MAX_OCCUR){
            // Copy the word
            strcpy(sequenceTable.sequences[i].occurences[occ].word, word);
            // Set position
            sequenceTable.sequences[i].occurences[occ].position = position;
            // Increment occurrences number
            sequenceTable.sequences[i].occurencesNumber++;
            }
        }
    }

    return sequenceTable;
}

Table resetTable(Table table)
{
    for (int i = 0; i < table.sequencesNumber; i++)
    {
        table.sequences[i].occurencesNumber = 0;
    }

    return table;
}

void printSequenceItem(SequenceItem sequence)
{

    printf("La sequenza: %s, e' contenuta in: \n", sequence.sequence);

    for (int i = 0; i < sequence.occurencesNumber; i++)
    {
        printf("\t%s - posizione: %d\n", sequence.occurences[i].word, sequence.occurences[i].position);
    }
}

void printTable(Table table)
{
    for (int i = 0; i < table.sequencesNumber; i++)
    {
        printSequenceItem(table.sequences[i]);
        printf("\n");
    }
}