#include "equipArray.h"

struct equipArray_s
{
    int n_objects;
    int max_objects;
    int *objects;
};
static void addToEquipment(equipArray_t equipArray, invArray_t inventary);
static void removeFromEquipment(equipArray_t equipArray, invArray_t inventary);
static void addObjectToPlayerEquipment(equipArray_t equipment, int newObj);
static void removeObjectToPlayerEquipment(equipArray_t equipment, int newObj);
static int isEquipmentStatisticsGood(equipArray_t equipment, invArray_t invArray);

/* creatore e disruttore */
equipArray_t equipArray_init()
{
    equipArray_t equipment = malloc(sizeof(*equipment));

    equipment->n_objects = 0;
    equipment->max_objects = EQUIP_SLOT;
    equipment->objects = (int *)malloc(equipment->max_objects * sizeof(int));

    for (int i = 0; i < equipment->max_objects; i++)
        equipment->objects[i] = -1;

    return equipment;
}
void equipArray_free(equipArray_t equipArray)
{
    equipArray->n_objects = 0;
    equipArray->max_objects = 0;
    free(equipArray->objects);
    free(equipArray);
}

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray)
{
    return equipArray->n_objects;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray)
{
    printf("Equipaggio con oggetti in uso:%d\n", equipArray->n_objects);
    for (int i = 0; i < equipArray->max_objects; i++)
    {
        if (equipArray->objects[i] != -1)
        {
            printf("%d)", i + 1);
            invArray_printByIndex(fp, invArray, equipArray->objects[i]);
            printf("\n");
        }
    }
}
/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(FILE *fp, equipArray_t equipArray, invArray_t invArray)
{
    // Stampo equipaggiamento
    equipArray_print(fp, equipArray, invArray);

    int scelta = -1;

    fprintf(fp, "0) Aggiungi elemento \n1) Rimuovi elemento\n)");

    scanf("%d", &scelta);

    if (scelta == 1)
        addToEquipment(equipArray, invArray);
    else
    {
        removeFromEquipment(equipArray, invArray);
    }
}
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index)
{
    return equipArray->objects[index];
}

void addToEquipment(equipArray_t equipArray, invArray_t inventary)
{
    char scelta[51];
    // Scelta
    printf("Scegli il nome dell'oggetto da includere:\n");
    invArray_print(stdout, inventary);

    printf("Scelta: \t");
    scanf("%s", &scelta);

    int objectIndex = invArray_searchByName(inventary, scelta);

    if (objectIndex != -1)
    {

        addObjectToPlayerEquipment(equipArray, objectIndex);

        if (!isEquipmentStatisticsGood(equipArray, inventary))
        {
            printf("Inserimento non permesso per statistiche negative");
            removeObjectToPlayerEquipment(equipArray, objectIndex);
        }
    }
}
static void removeFromEquipment(equipArray_t equipArray, invArray_t inventary)
{
    char scelta[51];
    // Scelta
    printf("Scegli il nome dell'oggetto da eliminare:\n");
    equipArray_print(stdout, equipArray, inventary);

    printf("Scelta: \t");
    scanf("%s", &scelta);

    int objectIndex = invArray_searchByName(inventary, scelta);
    if (objectIndex != -1)
    {
        removeObjectToPlayerEquipment(equipArray, objectIndex);

        if (!isEquipmentStatisticsGood(equipArray, inventary))
        {
            printf("Eliminazione non permessa per statistiche negative");
            addObjectToPlayerEquipment(equipArray, objectIndex);
        }
    }
}
void addObjectToPlayerEquipment(equipArray_t equipment, int newObj)
{
    for (int i = 0; i < equipment->max_objects; i++)
    {
        if (equipment->objects[i] == -1)
        {
            equipment->objects[i] = newObj;
            equipment->n_objects++;
            break;
        }
    }
}
void removeObjectToPlayerEquipment(equipArray_t equipment, int newObj)
{
    for (int i = 0; i < equipment->max_objects; i++)
    {
        if (equipment->objects[i] == newObj)
        {
            equipment->objects[i] = -1;
            equipment->n_objects--;
            break;
        }
    }
}

stat_t equipArray_getTotalStatistics(equipArray_t equipArray, invArray_t invArray)
{
    stat_t statistics = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < equipArray->max_objects; i++)
    {
        if (equipArray->objects[i] != -1)
        {
            inv_t *newObj = invArray_getByIndex(invArray, equipArray->objects[i]);

            statistics.hp += newObj->stat.hp;
            statistics.mp += newObj->stat.mp;
            statistics.atk += newObj->stat.atk;
            statistics.def += newObj->stat.def;
            statistics.mag += newObj->stat.mag;
            statistics.spr += newObj->stat.spr;
        }
    }

    return statistics;
}

int isEquipmentStatisticsGood(equipArray_t equipment, invArray_t invArray)
{

    stat_t statsToCheck = equipArray_getTotalStatistics(equipment, invArray);

    if (statsToCheck.hp < 0 || statsToCheck.mp < 0 || statsToCheck.atk < 0 || statsToCheck.def < 0 || statsToCheck.mag < 0 || statsToCheck.spr < 0)
        return 0;

    return 1;
}