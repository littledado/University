#include "invArray.h"
#include "inv.h"
#define MAX_STR_LEN 50 + 1 // Dimensione per stringa

struct invArray_s
{
    int n_objects;
    int max_objects;
    inv_t *objects;
};

void reallocInventario(invArray_t *inventario);

/* creatore e disruttore */
invArray_t invArray_init()
{
    invArray_t inv = malloc(sizeof(*inv));

    inv->n_objects = 0;
    inv->max_objects = 1;
    inv->objects = (inv_t *)malloc(inv->max_objects * sizeof(inv_t));

    return inv;
}
void invArray_free(invArray_t invArray)
{
    free(invArray->objects);
    free(invArray);
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp_in, invArray_t inventario)
{
    char codice[MAX_STR_LEN], name[MAX_STR_LEN], class[MAX_STR_LEN];
    int hp, mp, atk, def, mag, spr;
    int n_record;

    fscanf(fp_in, "%d", &n_record);

    for (int i = 0; i < n_record; i++)
    {
        if (i >= inventario->n_objects)
            reallocInventario(&inventario);
        inv_t object;
        // Leggo nmome oggetto e tipologia
        fscanf(fp_in, "%s %s", object.nome, object.tipo);
        // leggo le statistiche
        stat_read(fp_in, &object.stat);

        inventario->objects[i] = object;
        inventario->n_objects++;
    }
}

void reallocInventario(invArray_t *inventario)
{
    (*inventario)->objects = realloc((*inventario)->objects, 2 * (*inventario)->max_objects * sizeof(inv_t));
    (*inventario)->max_objects = (*inventario)->max_objects * 2;
}

void invArray_print(FILE *fp, invArray_t invArray)
{

    printf("\nStampa dell'inventario. Presenti: %d oggetti", invArray->n_objects);

    for (int i = 0; i < invArray->n_objects; i++)
    {
        printf("%d)\t", i);
        inv_print(fp, &invArray->objects[i]);
        printf("\n");
    }
}
/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index)
{
    if (index != -1 && invArray->n_objects > index)
    {
        inv_print(fp, &invArray->objects[index]);
    }

    else
    {
        printf("Nessun oggetto trovato per l'indice specificato\n");
    }
}
/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t *invArray_getByIndex(invArray_t invArray, int index)
{
    if (index != -1 && invArray->n_objects > index)
        return &invArray->objects[index];
}
/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name)
{
    for (int i = 0; i < invArray->n_objects; i++)
        if (strcmp(name, invArray->objects[i].nome) == 0)
            return i;

    return -1;
}

/* Si possono aggiungere altre funzioni se ritenute necessarie */