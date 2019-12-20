#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 255
#define FILE_PATH "C:\\Users\\Davide Trenti\\Desktop\\Progetti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab05\\Esercizio1\\brani.txt"
#define FILE_PATH_2 "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab05\\Esercizio1\\brani.txt"
typedef struct t_amico
{
    int songsNumber;
    char **songs;
} Amico;

typedef struct t_amici
{
    Amico **friends;
    int friendsNumber;
} Amici;

/* --- FUNZIONI --- */

Amico *allocateFriendWithSongs(int songsNumber);
Amici *allocateFriends(int friendsNumber);
void freeFriends(Amico *friends);
void freeFriend(Amico *friend);
Amici *readAmiciAndPlayListFromFile(FILE *fp);
void printAmici(Amici *friends);
int getSongsNumberOfFriends(Amici *friends);
int createAllPlaylists(int pos, Amici *friends, char **sol, int n, int count);

int main()
{
    FILE *fp_in;
    Amici *amici = NULL;
    char **solution = NULL;
    // Apro il file
    fp_in = fopen(FILE_PATH_2, "r");

    // Leggo la struttura dati
    amici = readAmiciAndPlayListFromFile(fp_in);
    // Alloco la memoria per la soluzione
    solution = (char **)malloc(amici->friendsNumber * sizeof(char *));
    // Stampo la struttura dati
    // printAmici(amici);

    createAllPlaylists(0, amici, solution, amici->friendsNumber, 0);

    return EXIT_SUCCESS;
}

Amico *allocateFriendWithSongs(int songsNumber)
{
    Amico *friend = (Amico *)malloc(sizeof(Amico));

    friend->songsNumber = songsNumber;
    friend->songs = (char **)malloc(songsNumber * sizeof(char *));
    for (int i = 0; i < songsNumber; i++)
        friend->songs[i] = (char *)malloc(MAX_CHAR * sizeof(char));

    return friend;
}

Amici *allocateFriends(int friendsNumber)
{
    Amici *amici = (Amici *)malloc(sizeof(Amici));

    amici->friendsNumber = friendsNumber;

    amici->friends = (Amico **)malloc(friendsNumber * sizeof(Amico *));

    return amici;
}

Amici *readAmiciAndPlayListFromFile(FILE *fp)
{
    int friendsNumber = 0;
    Amici *friends;

    //Leggo il numero di amici presenti nel file
    fscanf(fp, "%d", &friendsNumber);
    // Allocate friends
    friends = allocateFriends(friendsNumber);

    for (int i = 0; i < friendsNumber; i++)
    {
        int songsNumber = 0;
        // Leggo il numero di canzoni di ogni singolo amico
        fscanf(fp, "%d", &songsNumber);
        // Assegno il puntatore all'0interno del vettore di amici allocandone la memoria corretta
        friends->friends[i] = allocateFriendWithSongs(songsNumber);

        for (int j = 0; j < songsNumber; j++)
        {
            // Leggo il titolo della canzone
            fscanf(fp, "%s", friends->friends[i]->songs[j]);
        }
    }

    return friends;
}

void printAmici(Amici *friends)
{
    printf("Numero di amici:\t %d \n", friends->friendsNumber);

    for (int i = 0; i < friends->friendsNumber; i++)
    {
        printf("\tAmico '%d', numero di canzoni: %d\n", i + 1, friends->friends[i]->songsNumber);

        for (int j = 0; j < friends->friends[i]->songsNumber; j++)
        {
            printf("\t\tCanzone %d:\t%s\n", j + 1, friends->friends[i]->songs[j]);
        }
        printf("\n");
    }
    return;
}

int getSongsNumberOfFriends(Amici *friends)
{
    int count = 0;

    for (int i = 0; i < friends->friendsNumber; i++)
    {
        count += friends->friends[i]->songsNumber;
    }

    return count;
}

int createAllPlaylists(int pos, Amici *friends, char **sol, int n, int count)
{

    // Caso di terminazione
    if (pos >= n)
    {
        printf("\nPlaylist numero: %d\n", count);
        for (int i = 0; i < n; i++)
        {
            printf("\t\tCanzone %d:\t%s\n", i + 1, sol[i]);
        }
        return count + 1;
    }

    // for(int i = 0; i< n; i++){
    //  if (mark[i] == 0){
    //      mark[i] = 1;
    //      sol[pos] = friends[i]
    //      createAllPlaylists(po)
    //  }
    // }

    for (int i = 0; i < friends->friends[pos]->songsNumber; i++)
    {
        sol[pos] = friends->friends[pos]->songs[i];
        count = createAllPlaylists(pos + 1, friends, sol, n, count);
    }
    return count;
}
