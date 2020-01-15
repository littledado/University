#ifndef __INVENTARIO_H // Effettuo verifica di inclusione
#define __INVENTARIO_H
#define MAX_STR_LEN 50 + 1 // Dimensione per la stringa
typedef struct t_stats
{
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} ObjectStatistics;

typedef struct t_obj
{
    char name[MAX_STR_LEN];
    char type[MAX_STR_LEN];
    ObjectStatistics statistics;
} ObjectOfEquipment;

typedef struct t_inventario
{
    int n_objects;
    int max_objects;
    ObjectOfEquipment *objects;
} Inventario;

// Legge lista di oggetti da un file
Inventario* readInventaryFromFile(char *filename);
// Stampa l'inventario
void printInventary(Inventario *Inventario);
// Cerca un oggetto per nome
ObjectOfEquipment *findObjectInventaryByName(Inventario *Inventario, char *key);
// Stampa i dettagli di un oggetto
void printObjectDetail(ObjectOfEquipment *object);
// genera le statistiche di un oggetto
ObjectStatistics generateObjectStatistics(int hp, int mp, int atk, int def, int mag, int spr);
#endif