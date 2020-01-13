#include <stdio.h>
#include <stdlib.h>
#define FILE_NAME_TILES "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio2\\tiles.txt"
#define FILE_NAME_BOARD "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio2\\board.txt"

typedef struct t_tubo
{
    char colour[2];
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
        fscanf(fp_in, "%s %d %s %d", &collection->tiles[i].horizontal.colour, &collection->tiles[i].horizontal.score,
               &collection->tiles[i].vertical.colour, &collection->tiles[i].vertical.score);
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