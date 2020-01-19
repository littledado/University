#include "pgList.h"

typedef struct t_pg_node
{
    pg_t player;
    struct t_pg_node *next;
} PgNode;

struct pgList_s
{
    int n_players;
    PgNode *head;
    PgNode *tail;
};

static void freeListRecursive(PgNode *head);
static PgNode *allocatePlayerNode(pg_t player, PgNode *next);
static void pgList_printRecursive(FILE *fp, PgNode *node, invArray_t invArray);
/* creatore e distruttore */
pgList_t pgList_init()
{
    pgList_t table = malloc(sizeof(*table));

    table->head = table->tail = NULL;

    table->n_players = 0;

    return table;
}
void pgList_free(pgList_t pgList)
{
    freeListRecursive(pgList->head);
    free(pgList->tail);
    free(pgList);
}

/* lettura e scrittura su file */
void pgList_read(FILE *fp, pgList_t pgList)
{
    pg_t player;
    
    while (pg_read(fp, &player))
    {
        pgList_insert(pgList, player);
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray)
{
    fprintf(fp, "Personaggi:\n");
    pgList_printRecursive(fp, pgList->head, invArray);
}

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg)
{
    // Caso di lista vuota
    if (pgList->head == NULL)
    {
        pgList->head = pgList->tail = allocatePlayerNode(pg, NULL);
        pgList->n_players++;

        return;
    }

    PgNode *oldTail = pgList->tail;

    pgList->tail = allocatePlayerNode(pg, NULL);
    oldTail->next = pgList->tail;
    pgList->n_players++;
}
/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char *cod)
{
    // Caso di lista vuota
    if (pgList->head == NULL)
        return;

    PgNode *currentNode, *previousNode;

    for (previousNode = NULL, currentNode = pgList->head; currentNode != NULL; previousNode = currentNode, currentNode = currentNode->next)
    {

        if (strcmp(currentNode->player.cod, cod) == 0)
        {
            PgNode *temp = currentNode;

            if (previousNode == NULL){
                pgList->head = currentNode->next;
            }
            else
                previousNode->next = temp->next;
            
            free(temp);
            pgList->n_players--;
            break;
        }
    }
}
/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char *cod)
{
    if (pgList->head == NULL)
        return NULL;
    PgNode *previousNode, *currentNode;

    for (previousNode = NULL, currentNode = pgList->head; currentNode != NULL; previousNode = currentNode, currentNode = currentNode->next)
    {
        if (strcmp(currentNode->player.cod, cod) == 0)
            return &currentNode->player;
    }

    return NULL;
}

/* Si possono aggiungere altre funzioni se ritenute necessarie */
static void freeListRecursive(PgNode *head)
{
    if (head == NULL)
        return;

    freeListRecursive(head->next);

    free(head);
}

static void pgList_printRecursive(FILE *fp, PgNode *node, invArray_t invArray)
{
    if (node == NULL)
        return;

    pg_print(fp, &node->player, invArray);

    pgList_printRecursive(fp, node->next, invArray);
}

static PgNode *allocatePlayerNode(pg_t player, PgNode *next)
{

    PgNode *newNode = (PgNode *)malloc(sizeof(*newNode));

    if (newNode == NULL)
        return NULL;

    newNode->player = player;
    newNode->next = next;

    return newNode;
}