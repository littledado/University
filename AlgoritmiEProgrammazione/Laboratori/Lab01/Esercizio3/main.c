#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 30
#define MAX_COMMAND 100

typedef enum
{
    SX,
    DX,
    UP,
    DOWN
} DirectionCommand;

typedef enum
{
    ROW,
    COLUMN,
    END
} SelectorCommand;

typedef struct
{
    int index;
    int rotationNumber;
    SelectorCommand selector;
    DirectionCommand direction;
} Command;

typedef struct
{
    int index;
    int rotationNumber;
    char selector[10];
    char direction[10];
} RawCommand;

typedef struct
{
    int value[MAX_N][MAX_N];
    int n_rows;
    int n_cols;
} Matrix;
Matrix readMatrixFromFile(FILE *fp);
Command readCommandFromUser(int nrows, int ncol);
RawCommand readRawCommandFromFile(FILE *fp);
Command createCommand(RawCommand command);
int isExitCommand(Command command);
void printMatrix(Matrix matrix);
Matrix executeCommand(Command commandToExecute, Matrix matrix);
void copyArray(int arrayFrom[], int arrayTo[], int dim);
int isCommandvalid(RawCommand commandToCheck, int nrows, int ncol);
void ruotaRiga(int inputArray[MAX_N], int N, DirectionCommand direction, int rotationNumber);
void copyArrayIntoMatrix(int array[MAX_N], int matrix[MAX_N][MAX_N], int n_rows, int index);
void ruotaColonna(int matrix[MAX_N][MAX_N], int n_rows, int n_col, int indexColumn, DirectionCommand direction, int rotationNumber);

int main()
{
    FILE *fp_in;
    Matrix matrix;
    Command commandToExecute;

    //Apro il file
    fp_in = fopen("C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab01\\Esercizio3\\matrix", "r");
    //Leggo l'array
    matrix = readMatrixFromFile(fp_in);

    //Leggo comando
    while (!isExitCommand(commandToExecute = readCommandFromUser(matrix.n_rows, matrix.n_cols)))
    {
        // Eseguo comando
        matrix = executeCommand(commandToExecute, matrix);
        // Stampo vettore
        printMatrix(matrix);
    }

    return 0;
}

Matrix readMatrixFromFile(FILE *fp)
{
    Matrix matrix;

    fscanf(fp, "%d %d", &matrix.n_rows, &matrix.n_cols);

    for (int i = 0; i < matrix.n_rows; i++)
    {
        for (int j = 0; j < matrix.n_cols; j++)
            fscanf(fp, "%d", &matrix.value[i][j]);
    }
    return matrix;
}

Command readCommandFromUser(int nrows, int ncol)
{
    RawCommand rawCommand;

    // Read command from terminal
    do
    {
        rawCommand = readRawCommandFromFile(stdin);
    } while (isCommandvalid(rawCommand, nrows, ncol) != 1);

    return createCommand(rawCommand);
}

RawCommand readRawCommandFromFile(FILE *fp)
{
    RawCommand command;
    char buffer[MAX_COMMAND];

    printf("Inserisci il comando nel formato: <selettore> <indice> <direzione> <posizioni>\n"
           "Selettore: 'riga' - 'colonna - 'fine'\n"
           "indice: intero di riga o colonna\n"
           "direzione: 'destra' - 'sinistra' - 'su' - 'giu'\n"
           "posizioni: numero di rotazioni da effettuare\n");

    //Get string from file
    fgets(buffer, MAX_COMMAND, fp);

    sscanf(buffer, "%s %d %s %d", command.selector, &command.index, command.direction, &command.rotationNumber);

    return command;
}

Command createCommand(RawCommand command)
{
    Command commandToReturn;

    commandToReturn.index = command.index;
    commandToReturn.rotationNumber = command.rotationNumber;

    if (strcmp(command.direction, "destra") == 0)
        commandToReturn.direction = DX;
    else if (strcmp(command.direction, "sinistra") == 0)
        commandToReturn.direction = SX;
    else if (strcmp(command.direction, "su") == 0)
        commandToReturn.direction = UP;
    else if (strcmp(command.direction, "giu") == 0)
        commandToReturn.direction = DOWN;

    if (strcmp(command.selector, "riga") == 0)
        commandToReturn.selector = ROW;
    else if (strcmp(command.selector, "colonna") == 0)
        commandToReturn.selector = COLUMN;
    else if (strcmp(command.selector, "fine") == 0)
        commandToReturn.selector = END;

    return commandToReturn;
}

int isExitCommand(Command command)
{
    if (command.selector == END)
        return 1;

    return 0;
}

void printMatrix(Matrix matrix)
{
    printf("Risultato:\n");

    for (int i = 0; i < matrix.n_rows; i++)
    {
        printf("[");

        for (int j = 0; j < matrix.n_cols; j++)
        {
            printf("%d", matrix.value[i][j]);

            if (j != matrix.n_cols - 1)
                printf(",");
        }
        printf("]\n");
    }
}

Matrix executeCommand(Command commandToExecute, Matrix matrix)
{
    if (commandToExecute.selector == ROW)
        ruotaRiga(matrix.value[commandToExecute.index - 1], matrix.n_cols, commandToExecute.direction, commandToExecute.rotationNumber);
    else if (commandToExecute.selector == COLUMN)
        ruotaColonna(matrix.value, matrix.n_rows, matrix.n_cols, commandToExecute.index, commandToExecute.direction, commandToExecute.rotationNumber);
    return matrix;
}

void copyArray(int arrayFrom[], int arrayTo[], int dim)
{
    for (int i = 0; i < dim; i++)
    {
        arrayTo[i] = arrayFrom[i];
    }
}

int isCommandvalid(RawCommand commandToCheck, int nrows, int ncol)
{
    if (strcmp(commandToCheck.selector, "riga") != 0 && strcmp(commandToCheck.selector, "colonna") != 0)
    {
        return 0;
    }

    if (strcmp(commandToCheck.direction, "destra") != 0 &&
        strcmp(commandToCheck.direction, "sinistra") != 0 &&
        strcmp(commandToCheck.direction, "su") != 0 &&
        strcmp(commandToCheck.direction, "giu") != 0)
    {
        return 0;
    }

    if (strcmp(commandToCheck.direction, "destra") == 0 || commandToCheck.direction, "sinistra" == 0)
    {
        if (commandToCheck.index - 1 >= nrows)
            return 0;
    }

    if (strcmp(commandToCheck.direction, "su") == 0 || commandToCheck.direction, "giu" == 0)
    {
        if (commandToCheck.index - 1 >= ncol)
            return 0;
    }

    return 1;
}

void ruotaRiga(int *inputArray, int N, DirectionCommand direction, int rotationNumber)
{
    int resultArray[MAX_N];
    for (int i = 0; i < N; i++)
    {
        int position = 0;
        if (direction == DX)
        {
            position = rotationNumber + i;

            if (position >= N)
                position = position % N;
        }
        else if (direction == SX)
        {
            position = i - rotationNumber;

            if (position < 0)
            {
                if (abs(position) > N)
                    position = N - abs(position) % N;
                else
                    position = (N - abs(position)) % N;
            }
            else
            {
                position = position % N;
            }
        }

        resultArray[position] = inputArray[i];
    }

    copyArray(resultArray, inputArray, N);
}

void ruotaColonna(int matrix[MAX_N][MAX_N], int n_rows, int n_col, int indexColumn, DirectionCommand direction, int rotationNumber)
{
    int resultArray[MAX_N];
    for (int i = 0; i < n_rows; i++)
    {
        int position = 0;
        if (direction == DOWN)
        {
            position = rotationNumber + i;

            if (position >= n_rows)
                position = position % n_rows;
        }
        else if (direction == UP)
        {
            position = i - rotationNumber;

            if (position < 0)
            {
                if (abs(position) > n_rows)
                    position = n_rows - abs(position) % n_rows;
                else
                    position = (n_rows - abs(position)) % n_rows;
            }
            else
            {
                position = position % n_rows;
            }
        }

        resultArray[position] = matrix[i][indexColumn - 1];
    }

    copyArrayIntoMatrix(resultArray, matrix, n_rows, indexColumn - 1);
}

void copyArrayIntoMatrix(int array[MAX_N], int matrix[MAX_N][MAX_N], int n_rows, int index)
{

    for (int i = 0; i < n_rows; i++)
    {
        matrix[i][index] = array[i];
    }
}