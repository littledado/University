#include "inventario.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static ObjectOfEquipment createObjectOfEquipment(char *name, char *type, int hp, int mp, int atk, int def, int mag, int spr);
static Inventario *allocaInventario(int n_obj);
static void reallocInventario(Inventario **inventario);
static void printStatistics(ObjectStatistics *statistics);

ObjectOfEquipment createObjectOfEquipment(char *name, char *type, int hp, int mp, int atk, int def, int mag, int spr)
{
    ObjectOfEquipment obj;

    strcpy(obj.name, name);
    strcpy(obj.type, type);
    obj.statistics = generateObjectStatistics(hp, mp, atk, def, mag, spr);

    return obj;
}

ObjectStatistics generateObjectStatistics(int hp, int mp, int atk, int def, int mag, int spr)
{
    ObjectStatistics newStats = {hp, mp, atk, def, mag, spr};

    return newStats;
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
    (*inventario)->objects = realloc((*inventario)->objects, 2 * (*inventario)->max_objects * sizeof(ObjectOfEquipment));
    (*inventario)->max_objects = (*inventario)->max_objects * 2;
}

void printInventary(Inventario *Inventario)
{

    printf("\nStampa dell'inventario. Presenti: %d oggetti", Inventario->n_objects);

    for (int i = 0; i < Inventario->n_objects; i++)
    {
        printf("%d)\t", i);
        printObjectDetail(&Inventario->objects[i]);
        printf("\n");
    }
}

void printObjectDetail(ObjectOfEquipment *object)
{
    printf("Name: %s, Type: %s - ", object->name, object->type);
    printStatistics(&object->statistics);
}

void printStatistics(ObjectStatistics *statistics)
{
    printf("[%d, %d, %d, %d, %d, %d]", statistics->hp, statistics->mp, statistics->atk, statistics->def, statistics->mag, statistics->spr);
}

ObjectOfEquipment *findObjectInventaryByName(Inventario *Inventario, char *key)
{
    for (int i = 0; i < Inventario->n_objects; i++)
        if (strcmp(key, Inventario->objects[i].name) == 0)
            return &Inventario->objects[i];

    return NULL;
}

Inventario *readInventaryFromFile(char *filename)
{
    FILE *fp_in = fopen(filename, "r");
    if (fp_in == NULL)
        return 0;

    char codice[MAX_STR_LEN], name[MAX_STR_LEN], class[MAX_STR_LEN];
    int hp, mp, atk, def, mag, spr;
    int n_record;
    Inventario *inventario = allocaInventario(1);

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

    return inventario;
}