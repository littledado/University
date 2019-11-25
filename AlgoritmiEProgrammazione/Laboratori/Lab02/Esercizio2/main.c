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

typedef struct
{
    int n_record;
    trip trips[MAX_ROWS];
} trip_table;

typedef enum
{
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
    r_fine
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
void executeCommand(comando_e comando, trip_table table);
data parseDataFromRaw(char data[]);
ora parseTimeFromRaw(char time[]);
void filterByDate(trip_table table);
void filterByDateAndDelay(trip_table table);
void filterDate(trip_table table, int delay);
void filterByDestination(trip_table table, destination_type destinationType);
void printTrip(trip trip);
void showTotalDelayByTripId(trip_table table);

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
            executeCommand(comando, tripTable);
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
    }

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

    printf("Comandi disponibili: 'date', 'partenza', 'capolinea', 'ritardo', 'ritardo_tot'  e  'fine'\n"
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

    return r_fine;
}
void executeCommand(comando_e comando, trip_table table)
{
    switch (comando)
    {
    case r_date:
        filterByDate(table);
        break;
    case r_partenza:
        filterByDestination(table, departure);
        break;
    case r_capolinea:
        filterByDestination(table, arrival);
        break;
    case r_ritardo:
        filterByDateAndDelay(table);
        break;
    case r_ritardo_tot:
        showTotalDelayByTripId(table);
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
                printTrip(table.trips[i]);
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
                printTrip(table.trips[i]);
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
            printTrip(table.trips[i]);
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

void printTrip(trip trip)
{
    printf("Viaggio: %s - Partenza: %s - Capolinea: %s - Data: %d/%d/%d - Ora P: %d:%d:%d - Ora A: %d:%d:%d - Ritardo: %d\n", trip.id_trip, trip.start_trip, trip.end_trip,
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
                        printTrip(table.trips[i]);
                }
                else
                {
                    printTrip(table.trips[i]);
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