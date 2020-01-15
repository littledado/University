#include "inventario.h"


ObjectOfEquipment createObjectOfEquipment(char *name, char *type, int hp, int mp, int atk, int def, int mag, int spr)
{
    ObjectOfEquipment obj;

    strcpy(obj.name, name);
    strcpy(obj.type, type);
    obj.statistics = generatePlayerStatistics(hp, mp, atk, def, mag, spr);

    return obj;
}