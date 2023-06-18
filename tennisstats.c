#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 100

typedef struct tennisstats
{
    char *name;
    char *country;
    bool retired;
    int titles;
    int subtitles;
    int age;
    int rank;
    int points;
    int numOfAO;
    int numOfW;
    int numOfFO;
    int numOfUSO;
    struct tennisstats *left, *right;
} TS;

TS *initialize()
{
    return NULL;
}

TS *createPlayer(char *name, char *country, bool retired, int titles, int subtitles, int age, int rank, int points)
{
    TS *newPlayer = (TS *)malloc(sizeof(TS));
    newPlayer->name = strdup(name);
    newPlayer->country = strdup(country);
    newPlayer->retired = retired;
    newPlayer->titles = titles;
    newPlayer->subtitles = subtitles;
    newPlayer->age = age;
    newPlayer->rank = rank;
    newPlayer->points = points;
    newPlayer->numOfAO = 0;
    newPlayer->numOfW = 0;
    newPlayer->numOfFO = 0;
    newPlayer->numOfUSO = 0;
    newPlayer->left = NULL;
    newPlayer->right = NULL;
    return newPlayer;
}

TS *insertPlayer(TS *ts, TS *player)
{
    if (ts == NULL)
    {
        return player;
    }
    if (strcmp(player->name, ts->name) < 0)
    {
        ts->left = insertPlayer(ts->left, player);
    }
    else
    {
        ts->right = insertPlayer(ts->right, player);
    }
    return ts;
}

TS *searchByName(TS *ts, char *name)
{
    if (ts == NULL || strcmp(name, ts->name) == 0)
    {
        return ts;
    }
    if (strcmp(name, ts->name) < 0)
    {
        return searchByName(ts->left, name);
    }
    return searchByName(ts->right, name);
}

TS *removePlayer(TS *ts, char *name)
{
    if (ts == NULL)
    {
        return ts;
    }
    if (strcmp(name, ts->name) < 0)
    {
        ts->left = removePlayer(ts->left, name);
    }
    else if (strcmp(name, ts->name) > 0)
    {
        ts->right = removePlayer(ts->right, name);
    }
    else
    {
        if (ts->left == NULL)
        {
            TS *temp = ts->right;
            free(ts);
            return temp;
        }
        else if (ts->right == NULL)
        {
            TS *temp = ts->left;
            free(ts);
            return temp;
        }
        TS *temp = ts->right;
        while (temp->left != NULL)
        {
            temp = temp->left;
        }
        ts->name = temp->name;
        ts->country = temp->country;
        ts->retired = temp->retired;
        ts->titles = temp->titles;
        ts->subtitles = temp->subtitles;
        ts->age = temp->age;
        ts->rank = temp->rank;
        ts->points = temp->points;
        ts->numOfAO = temp->numOfAO;
        ts->numOfW = temp->numOfW;
        ts->numOfFO = temp->numOfFO;
        ts->numOfUSO = temp->numOfUSO;
        ts->right = removePlayer(ts->right, temp->name);
    }
    return ts;
}

void printPlayer(TS *player)
{
    printf("Name: %s\n", player->name);
    printf("Country: %s\n", player->country);
    printf("Retired: %s\n", player->retired ? "Yes" : "No");
    printf("Titles: %d\n", player->titles);
    printf("Subtitles: %d\n", player->subtitles);
    printf("Age: %d\n", player->age);
    printf("Rank: %d\n", player->rank);
    printf("Points: %d\n", player->points);
    printf("Australian Open Titles: %d\n", player->numOfAO);
    printf("Wimbledon Titles: %d\n", player->numOfW);
    printf("French Open Titles: %d\n", player->numOfFO);
    printf("US Open Titles: %d\n", player->numOfUSO);
    printf("------------------------\n");
}

void freePlayers(TS *ts)
{
    if (ts != NULL)
    {
        freePlayers(ts->left);
        freePlayers(ts->right);
        free(ts->name);
        free(ts->country);
        free(ts);
    }
}

int comparePlayers(const void *a, const void *b)
{
    TS *playerA = *(TS **)a;
    TS *playerB = *(TS **)b;

    // Compare by titles in descending order
    return playerB->titles - playerA->titles;
}

void printPlayersDescending(TS *ts, bool retired)
{
    TS *players[MAX_LINE_LENGTH];
    int index = 0;

    void storePlayersDescendingHelper(TS * ts, TS * *players, int *index, bool retired)
    {
        if (ts != NULL)
        {
            storePlayersDescendingHelper(ts->left, players, index, retired);

            if ((retired && ts->retired) || (!retired && !ts->retired))
            {
                if (ts->titles > 0)
                {
                    players[*index] = ts;
                    (*index)++;
                }
            }

            storePlayersDescendingHelper(ts->right, players, index, retired);
        }
    }

    // Store players in descending order of titles
    storePlayersDescendingHelper(ts, players, &index, retired);

    // Sort players by titles in descending order
    qsort(players, index, sizeof(TS *), comparePlayers);

    // Print players in decreasing order
    for (int i = 0; i < index; i++)
    {
        printPlayer(players[i]);
    }

    printf("Total Players: %d\n", index);
}

void printPlayersByAccumulatedPoints(TS *ts)
{
    void calculateAccumulatedPoints(TS * ts, TS * *players, int *index)
    {
        if (ts != NULL)
        {
            calculateAccumulatedPoints(ts->left, players, index);

            if (ts->retired)
            {
                int accumulatedPoints = ts->titles * 2000 + ts->subtitles * 1200;
                ts->points = accumulatedPoints;
                players[*index] = ts;
                (*index)++;
            }

            calculateAccumulatedPoints(ts->right, players, index);
        }
    }

    int comparePlayers(const void *a, const void *b)
    {
        TS *playerA = *(TS **)a;
        TS *playerB = *(TS **)b;

        // Compare by accumulated points in descending order
        return playerB->points - playerA->points;
    }

    TS *players[MAX_LINE_LENGTH];
    int index = 0;
    calculateAccumulatedPoints(ts, players, &index);

    // Sort players by accumulated points in descending order
    qsort(players, index, sizeof(TS *), comparePlayers);

    // Print players in descending order of points
    for (int i = 0; i < index; i++)
    {
        printf("Player Name: %s\n", players[i]->name);
        printf("Accumulated Points: %d\n", players[i]->points);
        printf("------------------------\n");
    }
}

void printPlayersByGrandSlamRanking(TS *ts)
{
    void calculateGrandSlamRanking(TS * ts, TS * *players, int *index)
    {
        if (ts != NULL)
        {
            calculateGrandSlamRanking(ts->left, players, index);

            if (ts->rank > 0)
            {
                int grandSlamRanking = ts->numOfAO + ts->numOfW + ts->numOfFO + ts->numOfUSO;
                ts->rank = grandSlamRanking;
                players[*index] = ts;
                (*index)++;
            }

            calculateGrandSlamRanking(ts->right, players, index);
        }
    }

    int comparePlayers(const void *a, const void *b)
    {
        TS *playerA = *(TS **)a;
        TS *playerB = *(TS **)b;

        // Compare by grand slam ranking in descending order
        return playerB->rank - playerA->rank;
    }

    TS *players[MAX_LINE_LENGTH];
    int index = 0;
    calculateGrandSlamRanking(ts, players, &index);

    // Sort players by grand slam ranking in descending order
    qsort(players, index, sizeof(TS *), comparePlayers);

    // Print players in descending order of grand slam ranking (excluding players with ranking 0)
    for (int i = 0; i < index; i++)
    {
        if (players[i]->rank > 0)
        {
            printf("Player Name: %s\n", players[i]->name);
            printf("Grand Slam Ranking: %d\n", players[i]->rank);
            printf("------------------------\n");
        }
    }
}

TS *removeActivePlayersByCountry(TS *ts, const char *country)
{
    if (ts == NULL)
    {
        return NULL;
    }

    ts->left = removeActivePlayersByCountry(ts->left, country);
    ts->right = removeActivePlayersByCountry(ts->right, country);

    if (strcmp(ts->country, country) == 0 && !ts->retired)
    {
        // Player belongs to the specified country and is active, so remove it
        ts = removePlayer(ts, ts->name);
    }

    return ts;
}

void addDataOne(TS **ts)
{
    FILE *filePtr = fopen("files/jog_atuais.txt", "r");
    char fileLine[MAX_LINE_LENGTH];
    if (filePtr == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    while (fgets(fileLine, MAX_LINE_LENGTH, filePtr) != NULL)
    {
        // Remove newline character at the end, if present
        size_t len = strlen(fileLine);
        if (fileLine[len - 1] == '\n')
        {
            fileLine[len - 1] = '\0';
        }

        char *rank = strtok(fileLine, "\t");
        char *country = strtok(NULL, "\t");
        char *name = strtok(NULL, "\t");
        char *points = strtok(NULL, "\t");
        char *age = strtok(NULL, "\t");

        // Check if the current player already exists in the binary tree
        TS *existingPlayer = searchByName(*ts, name);
        if (existingPlayer == NULL)
        {
            // Insert the player into the binary tree
            *ts = insertPlayer(*ts, createPlayer(name, country, false, 0, 0, atoi(age), atoi(rank), atoi(points)));
        }
    }
    fclose(filePtr);
}

void addDataTwo(TS **ts)
{
    FILE *filePtr = fopen("files/era_aberta_grand_slams.txt", "r");
    char fileLine[MAX_LINE_LENGTH];
    if (filePtr == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    while (fgets(fileLine, MAX_LINE_LENGTH, filePtr) != NULL)
    {
        // Remove newline character at the end, if present
        size_t len = strlen(fileLine);
        if (fileLine[len - 1] == '\n')
        {
            fileLine[len - 1] = '\0';
        }

        char *year = strtok(fileLine, "\t");
        char *tournament = strtok(NULL, "\t");
        char *winner = strtok(NULL, "\t");
        char *runnerUp = strtok(NULL, "\t");

        // Check if the winner player already exists in the binary tree
        TS *existingWinner = searchByName(*ts, winner);
        if (existingWinner == NULL)
        {
            // Add a new player with the winner's data
            *ts = insertPlayer(*ts, createPlayer(winner, "unknown", true, 1, 0, 0, 0, 0));
            existingWinner = searchByName(*ts, winner); // Retrieve the newly inserted player
        }
        else
        {
            existingWinner->titles++;
        }

        // Check if the runner-up player already exists in the binary tree
        TS *existingRunnerUp = searchByName(*ts, runnerUp);
        if (existingRunnerUp == NULL)
        {
            // Add a new player with the runner-up's data
            *ts = insertPlayer(*ts, createPlayer(runnerUp, "unknown", true, 0, 1, 0, 0, 0));
            existingRunnerUp = searchByName(*ts, runnerUp); // Retrieve the newly inserted player
        }
        else
        {
            existingRunnerUp->subtitles++;
        }

        // Update the count of titles for the corresponding Grand Slam tournament
        if (strcmp(tournament, "Australian Open") == 0)
        {
            existingWinner->numOfAO++;
        }
        else if (strcmp(tournament, "Wimbledon") == 0)
        {
            existingWinner->numOfW++;
        }
        else if (strcmp(tournament, "French Open") == 0)
        {
            existingWinner->numOfFO++;
        }
        else if (strcmp(tournament, "U.S. Open") == 0)
        {
            existingWinner->numOfUSO++;
        }
    }

    fclose(filePtr);
}

int main()
{
    TS *ts = initialize();
    addDataOne(&ts); // initialize the tree with the data from jog_atuais.txt
    addDataTwo(&ts); // initialize the tree with the data from era_aberta_grand_slams.txt

    // printPlayersDescending(ts, false); // - A.1 (active players)
    // printPlayersDescending(ts, true);  // - A.2 (retired players)

    // printPlayersByAccumulatedPoints(ts); // C
    // printPlayersByGrandSlamRanking(ts);  // D
    // removeActivePlayersByCountry(ts, "[USA]"); - G

    // left - B, E, F + create Menu Interface
    freePlayers(ts);
    return 0;
}
