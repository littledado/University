#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 50 + 1
#define FILE_PATH "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab06\\Esercizio2\\anag1.txt"
typedef struct node *link;

typedef struct
{
    char prefix;
    char value[4 + 1];
} CodicePersona;

typedef struct
{
    int dd;
    int mm;
    int yyyy;
} Date;

typedef struct
{
    CodicePersona code;
    char name[MAX_CHAR];
    char surname[MAX_CHAR];
    char address[MAX_CHAR];
    char city[MAX_CHAR];
    int cap;
    Date dateOfBirth;
} Persona;

struct node
{
    Persona *persona;
    link next;
};

typedef enum
{
    FIND_ONLY,
    FIND_REMOVE
} FIND_TYPE;
typedef enum
{
    C_NONE,
    C_READ_FROM_USER,
    C_READ_FROM_FILE,
    C_SEARCH_BY_CODE,
    C_DELETE_BY_CODE,
    C_DELETE_BY_DATE_INTERVAL,
    C_PRINT_TO_FILE,
    C_EXIT
} Command;

Persona *
readPersonaFromUser();
void readAnagraficaFromFile(link *head, FILE *fp);
Persona readPersonaFromFile(FILE *fp);
Date dateParser(char rawDate[]);
Persona *createPersona(CodicePersona code, Date dateOfBirt, char name[], char surname[], char address[], char city[], int cap);
void personaDisplay(Persona *persona, FILE *fp_in);
CodicePersona createCodicePersona(char code[]);
void printAnagrafica(link head, FILE *fp_in);
link newNode(Persona *persona, link next);
void freePersona(Persona *p);
link listInsertSorted(link head, Persona *p);
// Returns -1 if D1 < D2 , 0 if D1 == D2, 1 if D1 > D2
int dateComparer(Date d1, Date d2);
// Returns link to element with key in input or NULL
Persona *listSearch(link head, CodicePersona key);
int codeComparer(CodicePersona code1, CodicePersona code2);
Persona *sortListDelete(link *head, CodicePersona key);
// Show menu and read command
Command showMenu();
// Function to exec command
void executeCommand(Command commandToExecute, FILE *fp_in, link *head);
void readAnagraficaFromUser(link *head);
void findPeopleByCode(link *head, FIND_TYPE findingMode);
int dateCompareWithInterval(Date dateToCheck, Date start, Date end);
Persona *sortListDeleteByDate(link *head, Date start, Date end);
void findPeopleByDate(link *head);
int main(void)
{
    link head = NULL;
    FILE *fp_in;
    Command comando = C_NONE;
    // Apro il file
    fp_in = fopen(FILE_PATH, "r");

    while ((comando = showMenu()) != C_EXIT)
    {
        executeCommand(comando, fp_in, &head);
    }

    return EXIT_SUCCESS;
}
void readAnagraficaFromUser(link *head)
{
    while (1)
    {
        int comandoRaw = 0;
        printf("Aggiungere un nuovo elemento?\n"
               "1) SI\n"
               "2) NO\n"
               "Scelta:\t");
        scanf("%d", &comandoRaw);

        if (comandoRaw == 1)
        {
            Persona *p = readPersonaFromUser();

            *head = listInsertSorted(*head, p);
        }
        else
            break;
    }
}
Persona *readPersonaFromUser()
{
    char code[5], name[MAX_CHAR], surname[MAX_CHAR], address[MAX_CHAR], city[MAX_CHAR], rawData[10 + 1];
    int cap;

    printf("Inserisci il codice della persona [AXXXX]:\t\t A");
    scanf("%s", code);
    printf("Inserisci il nome:\t");
    scanf("%s", name);
    printf("Inserisci il cognome:\t");
    scanf("%s", surname);
    printf("Inserisci la data di nascita (formato dd/mm/yyyy):\t");
    scanf("%s", rawData);
    printf("Inserisci indirizzo:\t");
    scanf("%s", address);
    printf("Inserisci citta:\t");
    scanf("%s", city);
    printf("Inserisci il CAP:\t");
    scanf("%d", &cap);

    return createPersona(createCodicePersona(code), dateParser(rawData), name, surname, address, city, cap);
}
Persona *createPersona(CodicePersona code, Date dateOfBirth, char name[], char surname[], char address[], char city[], int cap)
{
    Persona *persona = malloc(sizeof(*persona));

    persona->code = code;
    persona->dateOfBirth = dateOfBirth;
    strcpy(persona->name, name);
    strcpy(persona->surname, surname);
    strcpy(persona->address, address);
    strcpy(persona->city, city);
    persona->cap = cap;

    return persona;
}

void printAnagrafica(link head, FILE *fp_in)
{
    // Terminazione
    if (head == NULL)
        return;

    // Stampo l'item
    personaDisplay(head->persona, fp_in);

    printAnagrafica(head->next, fp_in);
}

void personaDisplay(Persona *persona, FILE *fp_in)
{
    fprintf(fp_in, "|-------------------------------------------|\n");
    fprintf(fp_in, "| Codice: %c%s, Nome: %s, Cognome: %s, Data di nascita: %d/%d/%d, Citta': %s, Via: %s, Cap: %d\n", persona->code.prefix,
            persona->code.value, persona->name, persona->surname, persona->dateOfBirth.dd,
            persona->dateOfBirth.mm, persona->dateOfBirth.yyyy, persona->city, persona->address, persona->cap);
}

CodicePersona createCodicePersona(char code[])
{
    CodicePersona codice;
    char *p = code;

    codice.prefix = 'A';

    // Riporto p ad essere puntatore alla stringa numerica
    if (!isdigit(*p))
        p++;

    strcpy(codice.value, p);
    return codice;
}

Date dateParser(char rawDate[])
{
    Date data_to_return;

    sscanf(rawDate, "%d/%d/%d", &data_to_return.dd, &data_to_return.mm, &data_to_return.yyyy);

    return data_to_return;
}

void readAnagraficaFromFile(link *head, FILE *fp)
{
    char code[6], name[MAX_CHAR], surname[MAX_CHAR], address[MAX_CHAR], city[MAX_CHAR], rawData[10 + 1];
    int cap;

    while (fscanf(fp, "%s %s %s %s %s %s %d", code, name, surname, rawData, address, city, &cap) == 7)
    {
        // Creo oggetto persona
        Persona *persona = createPersona(createCodicePersona(code), dateParser(rawData), name, surname, address, city, cap);
        // Inserisco l'elemento in anagrafica
        *head = listInsertSorted(*head, persona);
    }
}

void freePersona(Persona *p)
{
    free(p);
}
link newNode(Persona *persona, link next)
{
    link x = malloc(sizeof(*x));

    if (x == NULL)
        return NULL;

    x->persona = persona;
    x->next = next;

    return x;
}

link listInsertSorted(link head, Persona *p)
{
    link x, pred;
    // Estraggo la chiave di ordinamento
    Date key = p->dateOfBirth;
    // Se la lista è vuota oppure
    if (head == NULL || dateComparer(p->dateOfBirth, head->persona->dateOfBirth) == -1)
        return newNode(p, head);

    // Effettuo un ciclo for per trovare il corretto valore di X e PRED
    for (x = head->next, pred = head; x != NULL && dateComparer(p->dateOfBirth, x->persona->dateOfBirth) == 1; pred = x, x = x->next)
        ;

    pred->next = newNode(p, x);

    return head;
}

int dateComparer(Date d1, Date d2)
{
    if (d1.yyyy > d2.yyyy)
        return 1;
    else if (d1.yyyy == d2.yyyy)
    {
        if (d1.mm > d2.mm)
        {
            return 1;
        }
        else if (d1.mm == d2.mm)
        {
            if (d1.dd > d2.dd)
                return 1;
            if (d1.dd == d2.dd)
                return 0;
            if (d1.dd < d2.dd)
                return -1;
        }
        return -1;
    }

    return -1;
}

Persona *listSearch(link head, CodicePersona key)
{
    link x;

    for (x = head; x != NULL; x = x->next)
    {
        if (codeComparer(x->persona->code, key) == 0)
            return x->persona;
    }
    return NULL;
}

int codeComparer(CodicePersona code1, CodicePersona code2)
{
    return strcmp(code1.value, code2.value);
}

Persona *sortListDelete(link *head, CodicePersona key)
{
    link x, p;
    Persona *persona = NULL;

    if (*head == NULL)
        return NULL;

    for (x = *head, p = NULL; x != NULL; p = x, x = x->next)
    {
        if (codeComparer(x->persona->code, key) == 0)
        {
            if (x == *head)
                *head = x->next;
            else
                p->next = x->next;
            // Assegno item
            persona = x->persona;

            // Libero la memoria del puntatore
            free(x);
            break;
        }
    }

    return persona;
}

void findPeopleByCode(link *head, FIND_TYPE findingMode)
{
    char code[5];
    CodicePersona keyToSearch;
    Persona *persona = NULL;

    printf("Inserisci il codice della persona da cercare / eliminare nel formato AXXXX:\t\tA");
    scanf("%s", code);
    // genero il codice da cercare
    keyToSearch = createCodicePersona(code);
    // cerco
    persona = listSearch(*head, keyToSearch);

    if (persona == NULL)
        printf("Persona non trovata in anagrafica");
    else if (findingMode == FIND_ONLY)
        personaDisplay(persona, stdout);
    else if (findingMode == FIND_REMOVE)
    {
        persona = sortListDelete(head, keyToSearch);

        personaDisplay(persona, stdout);
    }
}

Command showMenu()
{
    int command = 0;
    printf("\n\nInserisci il numero del comando:\n"
           "1) Acquisisci elemento da tastiera\n"
           "2) Leggi anagrafica da file\n"
           "3) Ricerca per codice \n"
           "4) Cancella per codice\n"
           "5) Cancella per intervallo di date\n"
           "6) Stampa anagrafica\n"
           "Altro per uscire..\nComando:\t");
    scanf("%d", &command);

    switch (command)
    {
    case 1:
        return C_READ_FROM_USER;
    case 2:
        return C_READ_FROM_FILE;
    case 3:
        return C_SEARCH_BY_CODE;
    case 4:
        return C_DELETE_BY_CODE;
    case 5:
        return C_DELETE_BY_DATE_INTERVAL;
    case 6:
        return C_PRINT_TO_FILE;
    default:
        return C_EXIT;
    }
}

void executeCommand(Command commandToExecute, FILE *fp_in, link *head)
{
    if (commandToExecute == C_READ_FROM_USER)
        readAnagraficaFromUser(head);

    if (commandToExecute == C_READ_FROM_FILE)
        readAnagraficaFromFile(head, fp_in);
    if (commandToExecute == C_SEARCH_BY_CODE)
        findPeopleByCode(head, FIND_ONLY);
    if (commandToExecute == C_DELETE_BY_CODE)
        findPeopleByCode(head, FIND_REMOVE);
    if (commandToExecute == C_DELETE_BY_DATE_INTERVAL)
        findPeopleByDate(head);
    if (commandToExecute == C_PRINT_TO_FILE)
        printAnagrafica(*head, stdout);
}

Persona *sortListDeleteByDate(link *head, Date start, Date end)
{
    link x, p;
    Persona *persona = NULL;

    if (*head == NULL)
        return NULL;

    for (x = *head, p = NULL; x != NULL && dateCompareWithInterval(x->persona->dateOfBirth, start, end) <= 0; p = x, x = x->next)
    {
        if (dateCompareWithInterval(x->persona->dateOfBirth, start, end) == 0)
        {
            if (x == *head)
                *head = x->next;
            else
                p->next = x->next;
            // Assegno item
            persona = x->persona;

            // Libero la memoria del puntatore
            free(x);
            break;
        }
    }

    return persona;
}

int dateCompareWithInterval(Date dateToCheck, Date start, Date end)
{

    int temp;

    temp = dateComparer(dateToCheck, start);

    if (temp == -1)
        return -1;
    else if (temp == 0)
        return 0;
    else
    {
        temp = dateComparer(dateToCheck, end);

        if (temp == 1)
            return 1;

        return 0;
    }
}

void findPeopleByDate(link *head)
{
    char code[5];
    char dateRaw[10 + 1];
    Date start, end;
    Persona *persona = NULL;

    printf("Inserisci il intervallo di date in cui rimuovere dall'anagrafica\n"
           "Data inizio (gg/mm/aaaa):\t");
    scanf("%s", dateRaw);
    start = dateParser(dateRaw);
    printf("Data fine (gg/mm/aaaa):\t");
    scanf("%s", dateRaw);
    end = dateParser(dateRaw);

    // cerco
    while ((persona = sortListDeleteByDate(head, start, end)) != NULL)
    {
        personaDisplay(persona, stdout);
    }
}