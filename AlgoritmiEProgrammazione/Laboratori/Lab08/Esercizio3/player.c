#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static PgTable *createPlayersList();
static Pg createPlayer(char *codice, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr);
static PgCode createPlayerId(char *codice);
static int isEqualId(PgCode *code1, PgCode *code2);
static ObjectStatistics DifferenceWithNewObject(ObjectStatistics base, ObjectOfEquipment *newObj);
static int isStatisticsGood(ObjectStatistics *statsToCheck);
static void printPlayerEquipment(Pg *player, Inventario *inventary);
static PgNode *allocatePlayerNode(Pg player, PgNode *next);
static Equipment getEmptyEquipment();
static void insertPlayerInList(PgTable *table, Pg player);
static ObjectStatistics AdditionWithNewObject(ObjectStatistics base, ObjectOfEquipment *newObj);
static void removePlayerWithKey(PgTable *table, PgCode *pgCode);
static void removeObjectFromPlayer(Pg *player, Inventario *inventary);
static void addObjectToPlayer(Pg *player, Inventario *inventary);
static void addObjectToPlayerEquipment(Pg *player, int newObj);
static void removeObjectToPlayerEquipment(Pg *player, int newObj);

PgTable *createPlayersList()
{
    PgTable *table = (PgTable *)malloc(sizeof(PgTable));

    table->head = table->tail = NULL;

    table->n_players = 0;

    return table;
}

Pg createPlayer(char *codice, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr)
{
    Pg newPlayer;
    // Setto i valori
    newPlayer.id = createPlayerId(codice);
    strcpy(newPlayer.name, name);
    strcpy(newPlayer.playerClass, class);
    newPlayer.statistics = generateObjectStatistics(hp, mp, atk, def, mag, spr);
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

int isEqualId(PgCode *code1, PgCode *code2)
{
    return strcmp(code1->value, code2->value) == 0;
}

ObjectStatistics DifferenceWithNewObject(ObjectStatistics base, ObjectOfEquipment *newObj)
{
    base.hp -= newObj->statistics.hp;
    base.mp -= newObj->statistics.mp;
    base.atk -= newObj->statistics.atk;
    base.def -= newObj->statistics.def;
    base.mag -= newObj->statistics.mag;
    base.spr -= newObj->statistics.spr;

    return base;
}

int isStatisticsGood(ObjectStatistics *statsToCheck)
{
    if (statsToCheck->hp < 0 || statsToCheck->mp < 0 || statsToCheck->atk < 0 || statsToCheck->def < 0 || statsToCheck->mag < 0 || statsToCheck->spr < 0)
        return 0;

    return 1;
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

void printPlayerEquipment(Pg *player, Inventario *inventary)
{
    printf("Equipaggio del giocatore:\n");
    for (int i = 0; i < MAX_EQUIPMENT; i++)
    {
        if (player->equipment.objects[i] != -1)
        {
            printf("%d)", i + 1);
            printObjectDetail(&inventary->objects[player->equipment.objects[i]]);
            printf("\n");
        }
    }
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

PgTable *readPlayersFromFile(char *filename)
{
    FILE *fp_in = fopen(filename, "r");
    PgTable *table = createPlayersList();
    if (fp_in == NULL)
        return 0;

    char codice[MAX_STR_LEN], name[MAX_STR_LEN], class[MAX_STR_LEN];
    int hp, mp, atk, def, mag, spr;

    while (fscanf(fp_in, "%s %s %s %d %d %d %d %d %d", codice, name, class, &hp, &mp, &atk, &def, &mag, &spr) == 9)
    {
        insertPlayerInList(table, createPlayer(codice, name, class, hp, mp, atk, def, mag, spr));
    }
    fclose(fp_in);

    return table;
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

ObjectStatistics AdditionWithNewObject(ObjectStatistics base, ObjectOfEquipment *newObj)
{
    base.hp += newObj->statistics.hp;
    base.mp += newObj->statistics.mp;
    base.atk += newObj->statistics.atk;
    base.def += newObj->statistics.def;
    base.mag += newObj->statistics.mag;
    base.spr += newObj->statistics.spr;

    return base;
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

void removeObjectFromPlayer(Pg *player, Inventario *inventary)
{
    int n_obj;
    ObjectStatistics newStats;

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

void addObjectToPlayer(Pg *player, Inventario *inventary)
{
    int n_obj;
    ObjectStatistics newStats;

    // Scelta
    printf("Scegli il numero dell'oggetto da includere:\n");
    printInventary(inventary);

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

    player->equipment.objects[newObj - 1] = -1;
    player->equipment.n_objects--;
}
