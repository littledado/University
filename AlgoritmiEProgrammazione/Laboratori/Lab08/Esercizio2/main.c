#include <stdio.h>
#include <stdlib.h>
#define FILE_NAME_TILES "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio2\\tiles.txt"
#define FILE_NAME_BOARD "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio2\\board.txt"

typedef struct t_tubo
{
    char color[2];
    int score;
} Tubo;

typedef enum e_rotation
{
    UNDEFINED = -1,
    NONE = 0,
    ROTATED = 1
} ROTATION;

typedef struct t_tile
{
    int id;
    Tubo horizontal;
    Tubo vertical;
} Tile;

typedef struct t_tiles_collection
{
    Tile *tiles;
    int *mark;
    int n_tiles;
} TilesCollection;

typedef struct t_cell
{
    int tileIndex;
    ROTATION rotation;
} Cell;

typedef struct t_scacchiera
{
    Cell *cells;
    int rows;
    int columns;
} Scacchiera;

// Legge la collezione di tessere
TilesCollection *readTilesCollection(char *fileName);
// Legge la scacchiera
Scacchiera *readScacchiera(char *fileName, TilesCollection *collection);
// Indica se si tratta di cella vuota
int isEmptyCell(Cell *cell);
// Setta la cella come vuota
void setEmptyCell(Cell *cell);
// Ottiene il valore del tubo orizzontale
Tubo *getHorizontalTubo(Tile *tile, ROTATION rotation);
//Ottiene il valore del tubo verticale
Tubo *getVerticalTubo(Tile *tile, ROTATION rotation);
// Calcola lo score di una scacchiera
int calculateScore(Scacchiera *scacchiera, TilesCollection *tilesCollection);
// stampa la tessera
void printTile(Tile *tile, ROTATION rotation);
// Stampa la scacchiera
void printScacchiera(Scacchiera *scacchiera, TilesCollection *tilesCollection, int score);
// Algoritmo di scelta tessere
void disposeTilesOnBoard(TilesCollection *tilesCollection, Scacchiera *scacchiera);
// Algoritmo di scelta tessere ricorsivo
void disposeTilesOnBoardRecursively(int pos, TilesCollection *tilesCollection, Scacchiera *scacchiera, Scacchiera *solution, int *solutionScore, int k);
int main()
{
    TilesCollection *tilesCollection = NULL;
    Scacchiera *scacchiera = NULL;

    // Leggo le tessere dal file con la collection
    tilesCollection = readTilesCollection(FILE_NAME_TILES);
    if (tilesCollection == NULL)
        return EXIT_FAILURE;
    // Leggo la scacchiera
    scacchiera = readScacchiera(FILE_NAME_BOARD, tilesCollection);
    if (scacchiera == NULL)
        return EXIT_FAILURE;
    // Eseguo algoritmo
    disposeTilesOnBoard(tilesCollection, scacchiera);

    return EXIT_SUCCESS;
}

TilesCollection *readTilesCollection(char *fileName)
{
    FILE *fp_in = fopen(fileName, "r");

    if (fp_in == NULL)
        return NULL;
    TilesCollection *collection = (TilesCollection *)malloc(sizeof(TilesCollection));

    fscanf(fp_in, "%d", &collection->n_tiles);
    // Alloco la memoria per vettore tessere e mark
    collection->mark = (int *)calloc(collection->n_tiles, sizeof(int));
    collection->tiles = (Tile *)malloc(collection->n_tiles * sizeof(Tile));

    for (int i = 0; i < collection->n_tiles; i++)
    {
        collection->tiles[i].id = i;
        fscanf(fp_in, "%s %d %s %d", &collection->tiles[i].horizontal.color, &collection->tiles[i].horizontal.score,
               &collection->tiles[i].vertical.color, &collection->tiles[i].vertical.score);
    }

    fclose(fp_in);
    return collection;
}

Scacchiera *readScacchiera(char *fileName, TilesCollection *collection)
{
    FILE *fp_in = fopen(fileName, "r");

    if (fp_in == NULL)
        return NULL;

    Scacchiera *scacchiera = (Scacchiera *)malloc(sizeof(Scacchiera));
    fscanf(fp_in, "%d %d", &scacchiera->rows, &scacchiera->columns);
    // Alloco la memoria per le celle
    scacchiera->cells = (Cell *)malloc(scacchiera->rows * scacchiera->columns * sizeof(Cell));

    for (int i = 0; i < scacchiera->columns * scacchiera->rows; i++)
    {
        int index, rotation;

        fscanf(fp_in, "%d%*c%d", &index, &rotation);

        // Creo la cella
        scacchiera->cells[i].tileIndex = index;
        scacchiera->cells[i].rotation = rotation;
        // Marco la tessera come già usata
        if (!isEmptyCell(&scacchiera->cells[i]))
            collection->mark[i] = 1;
    }

    fclose(fp_in);
    return scacchiera;
}

int isEmptyCell(Cell *cell)
{
    if (cell->tileIndex == -1 && cell->rotation == UNDEFINED)
        return 1;

    return 0;
}

void disposeTilesOnBoard(TilesCollection *tilesCollection, Scacchiera *scacchiera)
{
    Scacchiera *solution = (Scacchiera *)malloc(sizeof(Scacchiera));
    solution->rows = scacchiera->rows;
    solution->columns = scacchiera->columns;
    solution->cells = (Cell *)malloc(solution->rows * solution->columns * sizeof(Cell));
    int solutionScore = 0;
    disposeTilesOnBoardRecursively(0, tilesCollection, scacchiera, solution, &solutionScore, scacchiera->columns * scacchiera->rows);

    // Stampo il risultato della soluzione
    printScacchiera(solution, tilesCollection, solutionScore);
}

void disposeTilesOnBoardRecursively(int pos, TilesCollection *tilesCollection, Scacchiera *scacchiera, Scacchiera *solution, int *solutionScore, int k)
{
    // Condizione di terminazione
    if (pos >= k)
    {
        // Calcolo il nuovo score
        int score = calculateScore(scacchiera, tilesCollection);

        if (score > *solutionScore)
        {
            for (int i = 0; i < k; i++)
                solution->cells[i] = scacchiera->cells[i];

            *solutionScore = score;
        }
        return;
    }

    // Se non si tratta di casella già scelta
    if (isEmptyCell(&scacchiera->cells[pos]))
    {
        for (int i = 0; i < tilesCollection->n_tiles; i++)
        {
            // Se non ho ancora preso l'elemento
            if (tilesCollection->mark[i] == 0)
            {
                // Marco la tessera
                tilesCollection->mark[i] = 1;
                // Assegno i dati della tessera alla cella senza rotazione
                scacchiera->cells[pos].tileIndex = i;
                scacchiera->cells[pos].rotation = NONE;
                // Ricorro
                disposeTilesOnBoardRecursively(pos + 1, tilesCollection, scacchiera, solution, solutionScore, k);
                // Ruoto la casella e ricorro
                scacchiera->cells[pos].rotation = ROTATED;
                disposeTilesOnBoardRecursively(pos + 1, tilesCollection, scacchiera, solution, solutionScore, k);
                // Backtrack
                setEmptyCell(&scacchiera->cells[pos]);
                // Smarco la tessera
                tilesCollection->mark[i] = 0;
            }
        }
    }
    else
        disposeTilesOnBoardRecursively(pos + 1, tilesCollection, scacchiera, solution, solutionScore, k);
}

void setEmptyCell(Cell *cell)
{
    cell->tileIndex = -1;
    cell->rotation = UNDEFINED;
}

int calculateScore(Scacchiera *scacchiera, TilesCollection *tilesCollection)
{
    int rows = scacchiera->rows;
    int columns = scacchiera->columns;
    int score = 0;
    // Calcolo valore dei tubi orizzontali prendendo come riferimento il primo colore, tanto gli altri dovranno essere uguali di conseguenza
    // Ciclo per controllare le righe
    for (int i = 0; i < rows; i++)
    {
        Tubo *horizontalTube = getHorizontalTubo(&tilesCollection->tiles[scacchiera->cells[i * columns].tileIndex], scacchiera->cells[i * columns].rotation);
        int tempScore = 0;
        for (int j = 0; j < columns; j++)
        {
            Tubo *actualTube = getHorizontalTubo(&tilesCollection->tiles[scacchiera->cells[i * columns + j].tileIndex], scacchiera->cells[i * columns].rotation);
            if (horizontalTube->color[0] == actualTube->color[0])
                tempScore += actualTube->score;
            else
            {
                tempScore = 0;
                break;
            }
        }
        score += tempScore;
    }
    // Ciclo per controllare le colonne
    for (int i = 0; i < columns; i++)
    {
        Tubo *verticalTube = getVerticalTubo(&tilesCollection->tiles[scacchiera->cells[i].tileIndex], scacchiera->cells[i].rotation);
        int tempScore = 0;
        for (int j = 0; j < columns; j++)
        {
            Tubo *actualTube = getVerticalTubo(&tilesCollection->tiles[scacchiera->cells[i + j * columns].tileIndex], scacchiera->cells[i + j * columns].rotation);
            if (verticalTube->color[0] == actualTube->color[0])
                tempScore += actualTube->score;
            else
            {
                tempScore = 0;
                break;
            }
        }
        score += tempScore;
    }
    return score;
}

Tubo *getHorizontalTubo(Tile *tile, ROTATION rotation)
{
    if (rotation == ROTATED)
        return &tile->vertical;

    return &tile->horizontal;
}
Tubo *getVerticalTubo(Tile *tile, ROTATION rotation)
{
    if (rotation == ROTATED)
        return &tile->horizontal;

    return &tile->vertical;
}

void printTile(Tile *tile, ROTATION rotation)
{
    Tubo *horizTube = getHorizontalTubo(tile, rotation);
    Tubo *verticalTube = getVerticalTubo(tile, rotation);

    printf("*%s | %d**%s - %d*", verticalTube->color, verticalTube->score, horizTube->color, horizTube->score);
}

void printScacchiera(Scacchiera *scacchiera, TilesCollection *tilesCollection, int score)
{
    printf("Scacchiera con punteggio:\t%d", score);

    for (int i = 0; i < scacchiera->rows * scacchiera->columns; i++)
    {
        // Se devo cambiare riga
        if (i % scacchiera->columns == 0)
        {
            printf("\n");
        }

        printTile(&tilesCollection->tiles[scacchiera->cells[i].tileIndex], scacchiera->cells[i].rotation);

        printf(" .. ");
    }
    printf("\n");
}