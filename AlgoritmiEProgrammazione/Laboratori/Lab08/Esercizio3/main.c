#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"
#include "player.h"
#define FILE_NAME_PLAYERS "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio3\\pg.txt"
#define FILE_NAME_OBJECTS "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab08\\Esercizio3\\inventario.txt"

void showMenu(PgTable *table, Inventario *inventario);

int main()
{
    // Leggo la tabella da file
    PgTable *playersList = readPlayersFromFile(FILE_NAME_PLAYERS);
    // Leggo l'inventario da file
    Inventario *inventario = readInventaryFromFile(FILE_NAME_OBJECTS);

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
