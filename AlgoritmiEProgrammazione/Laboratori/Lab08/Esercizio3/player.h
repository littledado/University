#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "inventario.h"
#define MAX_EQUIPMENT 8

typedef struct t_codice
{
    char prefix[2 + 1];
    char value[4 + 1];
} PgCode;

typedef struct t_equipment
{
    int n_objects;
    int objects[MAX_EQUIPMENT];
} Equipment;
typedef struct t_pg
{
    PgCode id;
    char name[MAX_STR_LEN];
    char playerClass[MAX_STR_LEN];
    Equipment equipment;
    ObjectStatistics statistics;
} Pg;

typedef struct t_pg_node
{
    Pg player;
    struct t_pg_node *next;
} PgNode;

typedef struct
{
    int n_players;
    PgNode *head;
    PgNode *tail;
} PgTable;

// Cerca il personaggio dato il codice
Pg *findPlayer(PgTable *table, PgCode key);
// Stampa i dettagli di un personaggio
void printPlayerDetails(Pg *player, Inventario *inventario);
// Inserisce un personaggio all'interno del gioco
void readNewPlayerFromUser(PgTable *table);
// Rimuove un personaggio dal gioco
void removePlayerFromUser(PgTable *table);
// Legge i giocatori da un file
PgTable *readPlayersFromFile(char *filename);
// Elimina un elemento dal giocatore
void removeObject(PgTable *table, Inventario *inventario);
// Aggiunge un elemento al giocatore
void addObject(PgTable *table, Inventario *inventario);

#endif