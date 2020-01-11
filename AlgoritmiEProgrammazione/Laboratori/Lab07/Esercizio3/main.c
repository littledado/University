#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_PLAYERS "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab07\\Esercizio3\\pg.txt"
#define FILE_NAME_OBJECTS "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab07\\Esercizio3\\inventario.txt"
#define MAX_STR_LEN 50 + 1 // Dimensione per un stringa
#define MAX_EQUIPMENT 8

typedef struct t_codice
{
    char prefix[2 + 1];
    char value[4 + 1];
} PgCode;

typedef struct t_stats
{
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} PgStatistics;
typedef struct t_equipment
{
    int n_objects;
    int objects[MAX_EQUIPMENT];
} Equipment;
typedef struct t_pg
{
    PgCode id;
    char name[MAX_STR_LEN];
    char class[MAX_STR_LEN];
    Equipment equipment;
    PgStatistics statistics;
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
typedef struct t_obj
{
    char name[MAX_STR_LEN];
    char type[MAX_STR_LEN];
    PgStatistics statistics;
} ObjectOfEquipment;
typedef struct t_inventario
{
    int n_objects;
    int max_objects;
    ObjectOfEquipment *objects;
} Inventario;

/* Inizio prototipi di funzione */

// Crea la lista di personaggi;
PgTable *createPlayersList();
// Alloca la memoria per un nuovo nodo Player
PgNode *allocatePlayerNode(Pg player, PgNode *next);
// Create player ID
PgCode createPlayerId(char *codice);
// Crea il player
Pg createPlayer(char *codice, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr);
// Genera le statistiche del player
PgStatistics generatePlayerStatistics(int hp, int mp, int atk, int def, int mag, int spr);
// Get empty eq
Equipment getEmptyEquipment();
// Insert player in list
void insertPlayerInList(PgTable *table, Pg player);
// Leggo la lista di giocatori da file
int readPlayersListFromTable(char *filename, PgTable *table);
// Legge un nuovo giocatore
void readNewPlayerFromUser(PgTable *table);
// Rimuove un giocatore
void removePlayerFromUser(PgTable *table);
void removePlayerWithKey(PgTable *table, PgCode *PgCode);
// return 1 if two codes are equals
int isEqualId(PgCode *code1, PgCode *code2);
// Creo l'oggetto
ObjectOfEquipment createObjectOfEquipment(char *name, char *type, int hp, int mp, int atk, int def, int mag, int spr);
// Alloca l'inventario
Inventario *allocaInventario(int n_obj);
// Ridimensione l'inventario
void reallocInventario(Inventario **inventario);
// Stampa l'inventario
void printInventario(Inventario *Inventario);
// Stampa oggetto dell'inventario
void printObjectInEquipment(ObjectOfEquipment *object);
// Stampa le statistiche
void printStatistics(PgStatistics *stats);
// Calcola differenza con nuovo oggetto
PgStatistics DifferenceWithNewObject(PgStatistics base, ObjectOfEquipment *newObj);
void addObjectToPlayerEquipment(Pg *player, int newObj);
void printPlayerEquipment(Pg *player, Inventario *inventary);
void removeObjectFromPlayer(Pg *player, Inventario *inventary);
void showMenu(PgTable *table, Inventario *inventario);
Pg *findPlayer(PgTable *table, PgCode key);
void removeObject(PgTable *table, Inventario *inventario);
void addObject(PgTable *table, Inventario *inventario);
int readObjectsListFromTable(char *filename, Inventario *inventario);
int main()
{
    // Alloco la memoria per la lista di giocatori
    PgTable *playersList = createPlayersList();
    Inventario *inventario = allocaInventario(1);
    // Leggo la tabella da file
    readPlayersListFromTable(FILE_NAME_PLAYERS, playersList);
    // Leggo l'inventario
    readObjectsListFromTable(FILE_NAME_OBJECTS, inventario);
    showMenu(playersList, inventario);

    return EXIT_SUCCESS;
}
void showMenu(PgTable *table, Inventario *inventario)
{
    int scelta = 0;
    while (scelta != 6)
    {
        printf("1)Aggiungi personaggio\n"
               "2)Elimina un personaggio\n"
               "3)Aggiungi oggetto all'equipaggiamento\n"
               "4)Rimuovi oggetto dall'equipaggiamento\n"
               "5)Stampa elenco giocatori\n"
               "6) Esci\n");
        printf("Inserisci scelta:\t");
        scanf("%d", &scelta);

        switch (scelta)
        {
        case 1:
            readNewPlayerFromUser(table);
            break;
        case 2:
            removePlayerFromUser(table);
            break;
        case 3:
            addObject(table, inventario);
            break;
        case 4:
            removeObject(table, inventario);
            break;
        default:
            break;
        }
    }
}

PgTable *createPlayersList()
{
    PgTable *table = (PgTable *)malloc(sizeof(PgTable));

    table->head = table->tail = NULL;

    table->n_players = 0;

    return table;
}

PgNode *allocatePlayerNode(Pg player, PgNode *next)
{
    PgNode *newNode = (PgNode *)malloc(sizeof(*newNode));

    if (newNode == NULL)
        return NULL;

    newNode->player = player;
    newNode->next = next;

    return newNode;
}

Pg createPlayer(char *codice, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr)
{
    Pg newPlayer;
    // Setto i valori
    newPlayer.id = createPlayerId(codice);
    strcpy(newPlayer.name, name);
    strcpy(newPlayer.class, class);
    newPlayer.statistics = generatePlayerStatistics(hp, mp, atk, def, mag, spr);
    newPlayer.equipment = getEmptyEquipment();

    return newPlayer;
}

PgCode createPlayerId(char *codice)
{
    char buf[5];
    PgCode newCode = {"PG\0", "\0"};
    sscanf(codice, "PG%s", &buf);

    strcpy(newCode.value, buf);

    return newCode;
}

PgStatistics generatePlayerStatistics(int hp, int mp, int atk, int def, int mag, int spr)
{
    PgStatistics newStats = {hp, mp, atk, def, mag, spr};

    return newStats;
}

Equipment getEmptyEquipment()
{
    Equipment equipment = {0};
    for (int i = 0; i < MAX_EQUIPMENT; i++)
    {
        equipment.objects[i] = -1;
    }

    return equipment;
}

void insertPlayerInList(PgTable *table, Pg player)
{
    // Caso di lista vuota
    if (table->head == NULL)
    {
        table->head = table->tail = allocatePlayerNode(player, NULL);
        table->n_players++;

        return;
    }

    PgNode *oldTail = table->tail;

    table->tail = allocatePlayerNode(player, NULL);
    oldTail->next = table->tail;
    table->n_players++;
}

int readPlayersListFromTable(char *filename, PgTable *table)
{
    FILE *fp_in = fopen(filename, "r");
    if (fp_in == NULL)
        return 0;

    char codice[MAX_STR_LEN], name[MAX_STR_LEN], class[MAX_STR_LEN];
    int hp, mp, atk, def, mag, spr;

    while (fscanf(fp_in, "%s %s %s %d %d %d %d %d %d", codice, name, class, &hp, &mp, &atk, &def, &mag, &spr) == 9)
    {
        insertPlayerInList(table, createPlayer(codice, name, class, hp, mp, atk, def, mag, spr));
    }
    fclose(fp_in);

    return 1;
}

void readNewPlayerFromUser(PgTable *table)
{
    char codice[MAX_STR_LEN], name[MAX_STR_LEN], class[MAX_STR_LEN];
    int hp, mp, atk, def, mag, spr;
    char buff[5 * MAX_STR_LEN];

    printf("\nInserisci i dati del nuovo giocatore nel seguente formato: \n");
    printf("PG<XXXX> <nome> <cognome> <classe> <hp> <mp> <atk> <def> <mag> <spr>\n");
    fflush(stdin);
    fgets(buff, 5 * MAX_STR_LEN, stdin);

    sscanf(buff, "%s %s %s %d %d %d %d %d %d", codice, name, class, &hp, &mp, &atk, &def, &mag, &spr);

    insertPlayerInList(table, createPlayer(codice, name, class, hp, mp, atk, def, mag, spr));
}

void removePlayerWithKey(PgTable *table, PgCode *pgCode)
{
    // Caso di lista vuota
    if (table->head == NULL)
        return;

    PgNode *currentNode, *previousNode;

    for (previousNode = NULL, currentNode = table->head; currentNode->next != NULL; previousNode = currentNode, currentNode = currentNode->next)
    {

        if (isEqualId(&currentNode->player.id, pgCode))
        {
            PgNode *temp = currentNode;

            if (previousNode == NULL)
                table->head = currentNode->next;

            else
                previousNode->next = temp->next;

            free(temp);
            table->n_players--;
            break;
        }
    }
}

int isEqualId(PgCode *code1, PgCode *code2)
{
    return strcmp(code1->value, code2->value) == 0;
}

void removePlayerFromUser(PgTable *table)
{
    char buffer[MAX_STR_LEN];
    PgCode codeToRemove;
    printf("Inserisci codice del giocatore da eliminare (formato PGXXXX):\t PG");
    scanf("%s", buffer);

    strcpy(codeToRemove.value, buffer);

    removePlayerWithKey(table, &codeToRemove);
}

ObjectOfEquipment createObjectOfEquipment(char *name, char *type, int hp, int mp, int atk, int def, int mag, int spr)
{
    ObjectOfEquipment obj;

    strcpy(obj.name, name);
    strcpy(obj.type, type);
    obj.statistics = generatePlayerStatistics(hp, mp, atk, def, mag, spr);

    return obj;
}

Inventario *allocaInventario(int n_obj)
{
    Inventario *inv = (Inventario *)malloc(sizeof(Inventario));

    inv->n_objects = 0;
    inv->max_objects = n_obj;
    inv->objects = (ObjectOfEquipment *)malloc(n_obj * sizeof(ObjectOfEquipment));

    return inv;
}

void reallocInventario(Inventario **inventario)
{
    // inventario->objects = realloc(inventario->objects, 2 * inventario->max_objects);
    // inventario->max_objects = inventario->max_objects * 2;
    (*inventario)->objects = realloc((*inventario)->objects, 2 * (*inventario)->max_objects * sizeof(ObjectOfEquipment));
    (*inventario)->max_objects = (*inventario)->max_objects * 2;
}

void printInventario(Inventario *inventario)
{

    printf("\nStampa dell'inventario. Presenti: %d oggetti", inventario->n_objects);

    for (int i = 0; i < inventario->n_objects; i++)
    {
        printf("%d)\t", i);
        printObjectInEquipment(&inventario->objects[i]);
        printf("\n");
    }
}

void printObjectInEquipment(ObjectOfEquipment *object)
{
    printf("Name: %s, Type: %s - ", object->name, object->type);
    printStatistics(&object->statistics);
}

void printStatistics(PgStatistics *statistics)
{
    printf("[%d, %d, %d, %d, %d, %d]", statistics->hp, statistics->mp, statistics->atk, statistics->def, statistics->mag, statistics->spr);
}

PgStatistics DifferenceWithNewObject(PgStatistics base, ObjectOfEquipment *newObj)
{
    base.hp -= newObj->statistics.hp;
    base.mp -= newObj->statistics.mp;
    base.atk -= newObj->statistics.atk;
    base.def -= newObj->statistics.def;
    base.mag -= newObj->statistics.mag;
    base.spr -= newObj->statistics.spr;

    return base;
}

PgStatistics AdditionWithNewObject(PgStatistics base, ObjectOfEquipment *newObj)
{
    base.hp += newObj->statistics.hp;
    base.mp += newObj->statistics.mp;
    base.atk += newObj->statistics.atk;
    base.def += newObj->statistics.def;
    base.mag += newObj->statistics.mag;
    base.spr += newObj->statistics.spr;

    return base;
}

int isStatisticsGood(PgStatistics *statsToCheck)
{
    if (statsToCheck->hp < 0 || statsToCheck->mp < 0 || statsToCheck->atk < 0 || statsToCheck->def < 0 || statsToCheck->mag < 0 || statsToCheck->spr < 0)
        return 0;

    return 1;
}

void addObjectToPlayer(Pg *player, Inventario *inventary)
{
    int n_obj;
    PgStatistics newStats;

    // Scelta
    printf("Scegli il numero dell'oggetto da includere:\n");
    printInventario(inventary);

    printf("Scelta: \t");
    scanf("%d", &n_obj);

    newStats = AdditionWithNewObject(player->statistics, &inventary->objects[n_obj]);

    if (isStatisticsGood(&newStats))
    {
        addObjectToPlayerEquipment(player, n_obj);
        player->statistics = newStats;
    }
}

void addObjectToPlayerEquipment(Pg *player, int newObj)
{
    int actualN = player->equipment.n_objects;

    player->equipment.objects[actualN] = newObj;
    player->equipment.n_objects++;
}
void removeObjectToPlayerEquipment(Pg *player, int newObj)
{
    int actualN = player->equipment.n_objects;

    player->equipment.objects[newObj-1] = -1;
    player->equipment.n_objects--;
}
void removeObjectFromPlayer(Pg *player, Inventario *inventary)
{
    int n_obj;
    PgStatistics newStats;

    // Scelta
    printf("Scegli il numero dell'oggetto da includere:\n");
    printPlayerEquipment(player, inventary);

    printf("Scelta: \t");
    scanf("%d", &n_obj);

    newStats = DifferenceWithNewObject(player->statistics, &inventary->objects[n_obj]);

    if (isStatisticsGood(&newStats))
    {
        removeObjectToPlayerEquipment(player, n_obj);
        player->statistics = newStats;
    }
}

void printPlayerEquipment(Pg *player, Inventario *inventary)
{
    printf("Equipaggio del giocatore:\n");
    for (int i = 0; i < MAX_EQUIPMENT; i++)
    {
        if (player->equipment.objects[i] != -1)
        {
            printf("%d)", i + 1);
            printObjectInEquipment(&inventary->objects[player->equipment.objects[i]]);
            printf("\n");
        }
    }
}

Pg *findPlayer(PgTable *table, PgCode key)
{
    if (table->head == NULL)
        return NULL;
    PgNode *previousNode, *currentNode;

    for (previousNode = NULL, currentNode = table->head; currentNode != NULL; previousNode = currentNode, currentNode = currentNode->next)
    {
        if (isEqualId(&currentNode->player.id, &key))
            return &currentNode->player;
    }

    return NULL;
}

void removeObject(PgTable *table, Inventario *inventario)
{
    char buffer[MAX_STR_LEN];
    PgCode codeToRemove;
    printf("Inserisci codice del giocatore da modificare (formato PGXXXX):\t PG");
    scanf("%s", buffer);

    strcpy(codeToRemove.value, buffer);

    removeObjectFromPlayer(findPlayer(table, codeToRemove), inventario);
}

void addObject(PgTable *table, Inventario *inventario)
{
    char buffer[MAX_STR_LEN];
    PgCode codeToRemove;
    printf("Inserisci codice del giocatore da modificare (formato PGXXXX):\t PG");
    scanf("%s", buffer);

    strcpy(codeToRemove.value, buffer);

    addObjectToPlayer(findPlayer(table, codeToRemove), inventario);
}

int readObjectsListFromTable(char *filename, Inventario *inventario)
{
    FILE *fp_in = fopen(filename, "r");
    if (fp_in == NULL)
        return 0;

    char codice[MAX_STR_LEN], name[MAX_STR_LEN], class[MAX_STR_LEN];
    int hp, mp, atk, def, mag, spr;
    int n_record;

    fscanf(fp_in, "%d", &n_record);

    for (int i = 0; i < n_record; i++)
    {
        if (i >= inventario->n_objects)
            reallocInventario(&inventario);

        fscanf(fp_in, "%s %s %d %d %d %d %d %d", name, class, &hp, &mp, &atk, &def, &mag, &spr);

        inventario->objects[i] = createObjectOfEquipment(name, class, hp, mp, atk, def, mag, spr);
        inventario->n_objects++;
    }

    fclose(fp_in);

    return 1;
}