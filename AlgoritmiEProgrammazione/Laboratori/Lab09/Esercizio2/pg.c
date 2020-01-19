#include "pg.h"

/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pgp)
{
    pgp->equip = equipArray_init();
    return fscanf(fp, "%s %s %s %d %d %d %d %d %d", pgp->cod, pgp->nome, pgp->classe, &pgp->b_stat.hp, &pgp->b_stat.mp, &pgp->b_stat.atk, &pgp->b_stat.def, &pgp->b_stat.mag, &pgp->b_stat.spr) == 9;
}
/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(pg_t *pgp)
{
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray)
{
    fprintf(fp, "Codice giocatore: %s, Nome giocatore: %s\nClasse giocatore: %s\n", pgp->cod, pgp->nome, pgp->classe);

    if (equipArray_inUse(pgp->equip) > 0)
    equipArray_print(fp, pgp->equip, invArray);
}
/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(FILE *fp, pg_t *pgp, invArray_t invArray)
{
    fprintf(fp, "Modifica equipaggiamento del personaggio: %s", pgp->nome);

    equipArray_update(fp, pgp->equip, invArray);
}
