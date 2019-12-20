#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROWS 1000
#define MAX_CHAR 31
#define FILE_IN "C:\\Users\\david\\Desktop\\CodiciSorgenti\\University\\AlgoritmiEProgrammazione\\Laboratori\\Lab02\\Esercizio2\\corse.txt"

typedef struct
{
    int year;
    int month;
    int day;
} data;

typedef struct
{
    int h;
    int m;
    int s;
} ora;

typedef struct
{
    char id_trip[MAX_CHAR];
    char start_trip[MAX_CHAR];
    char end_trip[MAX_CHAR];
    data date_trip;
    ora time_departure;
    ora time_arrival;
    int delay;
} trip;

typedef struct
{
    char date_trip[MAX_CHAR];
    char time_departure[MAX_CHAR];
    char time_arrival[MAX_CHAR];
} trip_detail_raw;
typedef enum sortingType
{
    NONE = 0,
    BY_DATE,
    BY_CODE,
    BY_DEPARTURE,
    BY_ARRIVAL
} SortingType;
typedef struct
{
    int n_record;
    trip trips[MAX_ROWS];
    trip *tripsReference_DATE[MAX_ROWS];
    trip *tripsReference_CODE[MAX_ROWS];
    trip *tripsReference_DEPARTURE[MAX_ROWS];
    trip *tripsReference_ARRIVAL[MAX_ROWS];
    SortingType sorted;
} trip_table;

typedef enum
{
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
    r_fine,
    r_order_date,
    r_order_code,
    r_order_departure,
    r_order_arrival,
    r_print_file,
    r_print_terminal,
    r_find_departure
} comando_e;

typedef enum
{
    waiting_command,
    end_command
} command_status;
typedef enum
{
    arrival,
    departure
} destination_type;

void openFile(FILE **fp);
trip_table readTableFromFile(FILE *fp);
int isExitCommand(comando_e comando);
comando_e readCommandFromUser();
void executeCommand(comando_e comando, trip_table *table);
data parseDataFromRaw(char data[]);
ora parseTimeFromRaw(char time[]);
void filterByDate(trip_table table);
void filterByDateAndDelay(trip_table table);
void filterDate(trip_table table, int delay);
void filterByDestination(trip_table table, destination_type destinationType);
void printTrip(trip trip, FILE *fp);
void showTotalDelayByTripId(trip_table table);
void bubbleSort(trip *trips[MAX_ROWS], int dim, SortingType sortingMode);
int compareByDate(trip *trip1, trip *trip2);
int compareByCode(trip *trip1, trip *trip2);
int compareStrings(char *string1, char *string2);
int compareByDeparture(trip *trip1, trip *trip2);
int compareByArrival(trip *trip1, trip *trip2);
void printAllDataBase(trip_table *table, comando_e comando);
int linearStringSearch(trip_table *table, char *stringToCheck);
int dicotomicStringSearch(trip_table *table, char *stringToCheck);
int dicotomicStringSearchRec(trip *trips, int l, int r, char *stringToCheck);
void swap(trip *trip1, trip *trip2);
void searchByDeparture(trip_table *table);

int main()
{
    trip_table tripTable;
    comando_e comando;
    command_status command_status;

    FILE *fp_in;

    // Open file
    openFile(&fp_in);

    // Read table from file
    tripTable = readTableFromFile(fp_in);

    while (command_status == waiting_command)
    {
        // Read command from user
        comando = readCommandFromUser();

        if (isExitCommand(comando))
            command_status = end_command;
        else
        {
            // Execute command
            executeCommand(comando, &tripTable);
        }
    }

    return EXIT_SUCCESS;
}
void openFile(FILE **fp)
{
    *fp = fopen(FILE_IN, "r");
}
trip_table readTableFromFile(FILE *fp)
{
    trip_table table;
    fscanf(fp, "%d", &table.n_record);

    for (int i = 0; i < table.n_record; i++)
    {
        trip trip;
        trip_detail_raw trip_detail;

        fscanf(fp, "%s %s %s %s %s %s %d", &trip.id_trip, &trip.start_trip, &trip.end_trip,
               trip_detail.date_trip, trip_detail.time_departure, trip_detail.time_arrival, &trip.delay);

        trip.date_trip = parseDataFromRaw(trip_detail.date_trip);
        trip.time_departure = parseTimeFromRaw(trip_detail.time_departure);
        trip.time_arrival = parseTimeFromRaw(trip_detail.time_arrival);

        // save record
        table.trips[i] = trip;

        // Save pointers
        table.tripsReference_DATE[i] = table.tripsReference_CODE[i] = table.tripsReference_DEPARTURE[i] = table.tripsReference_ARRIVAL[i] = &table.trips[i];
    }
    // Set no sorting
    table.sorted = NONE;

    return table;
}
int isExitCommand(comando_e comando)
{
    if (comando == r_fine)
        return 1;

    return 0;
}

comando_e readCommandFromUser()
{
    char commandString[MAX_CHAR];

    printf("Comandi disponibili: 'date', 'partenza', 'capolinea', 'ritardo', 'ritardo_tot', 'ordina', 'stampa', 'cerca'  e  'fine'\n"
           "Inserisci comando:\t");
    scanf("%s", commandString);

    for (int i = 0; i < strlen(commandString); i++)
    {
        commandString[i] = toupper(commandString[i]);
    }

    if (strcmp(commandString, "DATE") == 0)
        return r_date;

    if (strcmp(commandString, "PARTENZA") == 0)
        return r_partenza;

    if (strcmp(commandString, "CAPOLINEA") == 0)
        return r_capolinea;

    if (strcmp(commandString, "RITARDO") == 0)
        return r_ritardo;

    if (strcmp(commandString, "RITARDO_TOT") == 0)
        return r_ritardo_tot;

    if (strcmp(commandString, "FINE") == 0)
        return r_fine;
    if (strcmp(commandString, "ORDINA") == 0)
    {
        int flag = 0;
        printf("1) Ordina per data\n"
               "2) Ordina per codice di tratta\n"
               "3) Ordina per stazione di partenza\n"
               "4) Ordina per stazione di arrivo\n"
               "Inserisci scelta:\t");

        scanf("%d", &flag);

        if (flag == 1)
            return r_order_date;
        if (flag == 2)
            return r_order_code;
        if (flag == 3)
            return r_order_departure;
        if (flag == 4)
            return r_order_arrival;
    }
    if (strcmp(commandString, "STAMPA") == 0)
    {
        int flag = 0;
        printf("1) Stampa su file out.txt\n"
               "2) Stampa su terminale\n"
               "Scelta:\t");

        scanf("%d", &flag);

        if (flag == 1)
            return r_print_file;
        if (flag == 2)
            return r_print_terminal;
    }
    if (strcmp(commandString, "CERCA") == 0)
        return r_find_departure;

    return r_fine;
}
void executeCommand(comando_e comando, trip_table *table)
{
    switch (comando)
    {
    case r_date:
        filterByDate(*table);
        break;
    case r_partenza:
        filterByDestination(*table, departure);
        break;
    case r_capolinea:
        filterByDestination(*table, arrival);
        break;
    case r_ritardo:
        filterByDateAndDelay(*table);
        break;
    case r_ritardo_tot:
        showTotalDelayByTripId(*table);
        break;
    case r_order_date:
        bubbleSort(table->tripsReference_DATE, table->n_record, BY_DATE);
        table->sorted = BY_DATE;
        break;
    case r_order_code:
        bubbleSort(table->tripsReference_CODE, table->n_record, BY_CODE);
        table->sorted = BY_CODE;
        break;
    case r_order_departure:
        bubbleSort(table->tripsReference_DEPARTURE, table->n_record, BY_DEPARTURE);
        table->sorted = BY_DEPARTURE;
        break;
    case r_order_arrival:
        bubbleSort(table->tripsReference_ARRIVAL, table->n_record, BY_ARRIVAL);
        table->sorted = BY_ARRIVAL;
        break;
    case r_print_file:
    case r_print_terminal:
        printAllDataBase(table, comando);
        break;
    case r_find_departure:
        searchByDeparture(table);
        break;
    default:
        break;
    }
}

void filterByDestination(trip_table table, destination_type destinationType)
{
    if (destinationType == departure)
    {
        char departure[MAX_CHAR];
        printf("Inserisci partenza:\t");
        scanf("%s", departure);

        for (int i = 0; i < table.n_record; i++)
        {
            if (strcmp(departure, table.trips[i].start_trip) == 0)
                printTrip(table.trips[i], stdout);
        }
    }

    if (destinationType == arrival)
    {
        char capolinea[MAX_CHAR];
        printf("Inserisci capolinea:\t");
        scanf("%s", capolinea);

        for (int i = 0; i < table.n_record; i++)
        {
            if (strcmp(capolinea, table.trips[i].end_trip) == 0)
                printTrip(table.trips[i], stdout);
        }
    }
}

void showTotalDelayByTripId(trip_table table)
{
    char tripId[MAX_CHAR];
    int totalDelay = 0;

    printf("Inserisci ID Tratta:\t");
    scanf("%s", tripId);

    for (int i = 0; i < table.n_record; i++)
    {
        if (strcmp(table.trips[i].id_trip, tripId) == 0)
        {
            // Pritn the trip
            printTrip(table.trips[i], stdout);
            // Add delay
            totalDelay += table.trips[i].delay;
        }
    }

    printf("Ritardo totale:\t%d minuti", totalDelay);
}

data parseDataFromRaw(char date[])
{
    data data_to_return;

    sscanf(date, "%d/%d/%d", &data_to_return.year, &data_to_return.month, &data_to_return.day);

    return data_to_return;
}

ora parseTimeFromRaw(char time[])
{
    ora time_to_return;

    sscanf(time, "%d:%d:%d", &time_to_return.h, &time_to_return.m, &time_to_return.s);

    return time_to_return;
}

void printTrip(trip trip, FILE *fp)
{
    fprintf(fp, "Viaggio: %s - Partenza: %s - Capolinea: %s - Data: %d/%d/%d - Ora P: %d:%d:%d - Ora A: %d:%d:%d - Ritardo: %d\n", trip.id_trip, trip.start_trip, trip.end_trip,
            trip.date_trip.year, trip.date_trip.month, trip.date_trip.day,
            trip.time_departure.h, trip.time_departure.m, trip.time_departure.s,
            trip.time_arrival.h, trip.time_arrival.m, trip.time_arrival.s,
            trip.delay);
}

// returns 1 if date 1 is newer than date2, -1 if date1 is minor than date2, 0 if date1 is equal date2
int dateCompare(data date1, data date2)
{
    if (date1.year < date2.year)
        return -1;

    if (date1.year > date2.year)
        return 1;

    if (date1.month < date2.month)
        return -1;

    if (date1.month > date2.month)
        return 1;

    if (date1.day < date2.day)
        return -1;

    if (date1.day > date2.day)
        return 1;

    return 0;
}

void filterDate(trip_table table, int delay)
{
    char data1_string[MAX_CHAR], data2_string[MAX_CHAR];
    data data1, data2;

    printf("Inserisci un intervallo di date:\n"
           "Data di partenza (aaaa/mm/dd):\t");
    scanf("%s", data1_string);

    printf("Data di arrivo (aaaa/mm/dd):\t");
    scanf("%s", data2_string);

    data1 = parseDataFromRaw(data1_string);
    data2 = parseDataFromRaw(data2_string);

    for (int i = 0; i < table.n_record; i++)
    {
        int arrivalComparation = dateCompare(table.trips[i].date_trip, data1);
        int departureComparation = dateCompare(table.trips[i].date_trip, data2);

        if (arrivalComparation == 0 || arrivalComparation == 1)
            if (departureComparation == 0 || departureComparation == -1)
            {
                if (delay)
                {
                    if (table.trips[i].delay > 0)
                        printTrip(table.trips[i], stdout);
                }
                else
                {
                    printTrip(table.trips[i], stdout);
                }
            }
    }
}

void filterByDateAndDelay(trip_table table)
{
    filterDate(table, 1);
}

void filterByDate(trip_table table)
{
    filterDate(table, 0);
}

// Algoritmo del bubble sort
void bubbleSort(trip *trips[MAX_ROWS], int dim, SortingType sortingMode)
{
    for (int i = 0; i < dim - 1; i++)
    {
        for (int j = 0; j < dim - i - 1; j++)
        {
            trip *tripCurrent = trips[j];
            trip *tripNext = trips[j + 1];
            int needToSwap = 0;
            switch (sortingMode)
            {
            case BY_DATE:
                needToSwap = compareByDate(tripCurrent, tripNext);
                break;
            case BY_CODE:
                needToSwap = compareByCode(tripCurrent, tripNext);
                break;
            case BY_ARRIVAL:
                needToSwap = compareByArrival(tripCurrent, tripNext);
                break;
            case BY_DEPARTURE:
                needToSwap = compareByDeparture(tripCurrent, tripNext);
            default:
                break;
            }

            // Scambio i valori in modo da ordinare la base dati
            if (needToSwap == 1)
                swap(tripCurrent, tripNext);
        }
    }
}

// Function to compare date. Returns 1 if greater
int compareByDate(trip *trip1, trip *trip2)
{
    if (dateCompare(trip1->date_trip, trip2->date_trip) == 1)
        return 1;

    return 0;
}

int compareStrings(char *string1, char *string2)
{
    return strcmp(string1, string2);
}
// Function to compare id trip. Returns 1 if greater
int compareByCode(trip *trip1, trip *trip2)
{
    return compareStrings(trip1->id_trip, trip2->id_trip);
}
// Function to compare departure. Returns 1 if greater
int compareByDeparture(trip *trip1, trip *trip2)
{
    return compareStrings(trip1->start_trip, trip2->start_trip);
}
// Function to compare arrivals. Returns 1 if greater
int compareByArrival(trip *trip1, trip *trip2)
{
    return compareStrings(trip1->end_trip, trip2->end_trip);
}

void swap(trip *trip1, trip *trip2)
{
    trip temp = *trip1;

    *trip1 = *trip2;
    *trip2 = temp;
}

void printAllDataBase(trip_table *table, comando_e comando)
{
    FILE *fp_out = NULL;

    if (comando == r_print_file)
        fp_out = fopen("out.txt", "w");
    else
        fp_out = stdout;

    for (int i = 0; i < table->n_record; i++)
    {
        switch (table->sorted)
        {
        case NONE:
            printTrip(table->trips[i], fp_out);
            break;
        case BY_DATE:
            printTrip(*table->tripsReference_DATE[i], fp_out);
            break;
        case BY_CODE:
            printTrip(*table->tripsReference_CODE[i], fp_out);
            break;
        case BY_DEPARTURE:
            printTrip(*table->tripsReference_DEPARTURE[i], fp_out);
            break;
        case BY_ARRIVAL:
            printTrip(*table->tripsReference_ARRIVAL[i], fp_out);
            break;
        default:
            break;
        }
    }

    if (comando == r_print_file)
        fclose(fp_out);
}

void searchByDeparture(trip_table *table)
{
    int index = -1;
    char stringToSearch[MAX_CHAR];

    printf("Inserisci partenza (anche parziale):\t");
    scanf("%s", stringToSearch);

    printf("Esito ricerca:\n");

    if (table->sorted == BY_DEPARTURE)
        index = dicotomicStringSearch(table, stringToSearch);
    else
        index = linearStringSearch(table, stringToSearch);

    if (index != -1)
        printTrip(table->trips[index], stdout);
    else
        printf("Nessun risultato trovato");
}

int linearStringSearch(trip_table *table, char *stringToCheck)
{
    for (int i = 0; i < table->n_record; i++)
    {
        if (strstr(table->trips[i].start_trip, stringToCheck) != NULL)
            return i;
    }

    return -1;
}

int dicotomicStringSearch(trip_table *table, char *stringToCheck)
{
    return dicotomicStringSearchRec(table->trips, 0, table->n_record - 1, stringToCheck);
}

int dicotomicStringSearchRec(trip *trips, int l, int r, char *stringToCheck)
{
    int sx = -1, dx = -1;

    if (l >= r)
    {
        if (strstr(trips[l].start_trip, stringToCheck) != NULL)
            return l;

        return -1;
    }

    sx = dicotomicStringSearchRec(trips, l, (l + r) / 2, stringToCheck);
    dx = dicotomicStringSearchRec(trips, (l + r) / 2 + 1, r, stringToCheck);

    if (sx != -1)
        return sx;
    if (dx != -1)
        return dx;
    return -1;
}