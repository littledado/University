#include <stdio.h>
#include <stdlib.h>

#define MAX_N 30

typedef enum
{
    SX = -1,
    DX = 1
} DirectionCommand;

typedef struct
{
    int rotationNumber;
    DirectionCommand direction;
} Command;

int readArrayDimensionFromUser(int maxDim);
void readArrayFromUser(int inputArray[], int dim);
Command readCommandFromUser();
Command createCommand(int n_pos, int dir);
int isNullCommand(Command command);
void printArray(int inputArray[], int dim);
void executeCommand(Command commandToExecute, int inputArray[], int N);
void copyArray(int arrayFrom[], int arrayTo[], int dim);

int main()
{
    int N = -1, inputArray[MAX_N];
    Command commandToExecute;

    //Leggo le dimensioni dell'array
    N = readArrayDimensionFromUser(MAX_N);

    //Leggo l'array
    readArrayFromUser(inputArray, N);

    //Leggo comando
    while (!isNullCommand(commandToExecute = readCommandFromUser()))
    {
        // Eseguo comando
        executeCommand(commandToExecute, inputArray, N);
        // Stampo vettore
        printArray(inputArray, N);
    }

    return 0;
}

int readArrayDimensionFromUser(int maxDim)
{
    int dim = 0;

    while (dim <= 0 || dim > maxDim)
    {
        printf("Inserisci dimensioni array:\t");
        scanf("%d", &dim);
    }

    return dim;
}

void readArrayFromUser(int inputArray[], int dim)
{
    printf("Inserisci elementi dell'array:\n");
    for (int i = 0; i < dim; i++)
    {
        printf("%d)\t", i + 1);
        scanf("%d", &inputArray[i]);
    }
}

Command readCommandFromUser()
{
    int n_pos = -1, dir = 0;
    printf("Inserisci il comando: ");

    while (n_pos < 0 || (dir != -1 && dir != 1))
    {
        printf("Inserisci il numero di posizioni da ruotare:\t");
        scanf("%d", &n_pos);
        printf("Inserisci la direzione (-1 SX e 1 DX):\t");
        scanf("%d", &dir);
    }

    return createCommand(n_pos, dir);
}

Command createCommand(int n_pos, int dir)
{
    Command commandToReturn;

    commandToReturn.direction = (DirectionCommand)dir;
    commandToReturn.rotationNumber = n_pos;

    return commandToReturn;
}

int isNullCommand(Command command)
{
    if (command.rotationNumber == 0)
        return 1;

    return 0;
}

void printArray(int inputArray[], int dim)
{
    printf("Risultato:\n");
    printf("[");

    for (int i = 0; i < dim; i++)
    {

        printf("%d", inputArray[i]);
        if (i != dim - 1)
            printf(",");
    }
    printf("]\n");
}

void executeCommand(Command commandToExecute, int inputArray[], int N)
{
    int resultArray[MAX_N];

    for (int i = 0; i < N; i++)
    {
        int position = 0;
        if (commandToExecute.direction == DX)
        {
            position = commandToExecute.rotationNumber + i;

            if (position >= N)
                position = position % N;
        }
        else if (commandToExecute.direction == SX)
        {
            position = i - commandToExecute.rotationNumber;

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

void copyArray(int arrayFrom[], int arrayTo[], int dim)
{
    for (int i = 0; i < dim; i++)
    {
        arrayTo[i] = arrayFrom[i];
    }
}