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
    int accpoints;
    int numOfAO;
    int numOfW;
    int numOfFO;
    int numOfUSO;
    int prevStreakCount;
    int streakYear;
    int streakCount;
    int mostTrophiesCount;
    int mostTrophiesYear;
    struct tennisstats *left, *right;
} TS;

TS *initialize()
{
    return NULL;
}

TS *createPlayer(char *name, char *country, bool retired, int titles, int subtitles, int age, int rank, int points, int streakYear, int streakCount, int prevStreakCount)
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
    newPlayer->accpoints = points;
    newPlayer->numOfAO = 0;
    newPlayer->numOfW = 0;
    newPlayer->numOfFO = 0;
    newPlayer->numOfUSO = 0;
    newPlayer->left = NULL;
    newPlayer->right = NULL;
    newPlayer->streakYear = streakYear;
    newPlayer->streakCount = streakCount;
    newPlayer->prevStreakCount = prevStreakCount;
    newPlayer->mostTrophiesCount = 0;
    newPlayer->mostTrophiesYear = 0;
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
    printf("------------------------\n");
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
    printf("Most Trophies Year: %d\n", player->mostTrophiesYear);
    printf("Most Trophies Count: %d\n", player->mostTrophiesCount);
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

void printRetiredPlayersByAccumulatedPoints(TS *ts)
{
    void calculateAccumulatedPoints(TS * ts, TS * *players, int *index)
    {
        if (ts != NULL)
        {
            calculateAccumulatedPoints(ts->left, players, index);

            if (ts->retired)
            {
                int accumulatedPoints = ts->titles * 2000 + ts->subtitles * 1200;
                ts->accpoints = accumulatedPoints;
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
        return playerB->accpoints - playerA->accpoints;
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
        printf("Accumulated Points: %d\n", players[i]->accpoints);
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
            *ts = insertPlayer(*ts, createPlayer(name, country, false, 0, 0, atoi(age), atoi(rank), atoi(points), 0, 0, 0));
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
            *ts = insertPlayer(*ts, createPlayer(winner, "unknown", true, 1, 0, 0, 0, 0, atoi(year), 1, 0));
            existingWinner = searchByName(*ts, winner); // Retrieve the newly inserted player
        }
        else
        {
            existingWinner->titles++;

            // Update streak and previous streak if necessary
            if (existingWinner->streakCount > existingWinner->prevStreakCount && existingWinner->streakYear != atoi(year))
            {
                existingWinner->prevStreakCount = existingWinner->streakCount;
            }

            if (existingWinner->streakYear == atoi(year))
            {
                existingWinner->streakCount++;
            }
            else
            {
                existingWinner->streakYear = atoi(year);
                existingWinner->streakCount = 1;
            }

            // Check if the current streak is the new most trophies year
            if (existingWinner->streakCount > existingWinner->mostTrophiesCount)
            {
                existingWinner->mostTrophiesCount = existingWinner->streakCount;
                existingWinner->mostTrophiesYear = existingWinner->streakYear;
            }
        }

        // Check if the runner-up player already exists in the binary tree
        TS *existingRunnerUp = searchByName(*ts, runnerUp);
        if (existingRunnerUp == NULL)
        {
            // Add a new player with the runner-up's data
            *ts = insertPlayer(*ts, createPlayer(runnerUp, "unknown", true, 0, 1, 0, 0, 0, 0, 0, 0));
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

void countCountriesHelper(TS *node, char ***countrySet, int *count)
{
    if (node == NULL)
    {
        return;
    }

    // Traverse left subtree
    countCountriesHelper(node->left, countrySet, count);

    // Add country to the set if it doesn't exist
    bool countryExists = false;
    for (int i = 0; i < *count; i++)
    {
        if (strcmp((*countrySet)[i], node->country) == 0)
        {
            countryExists = true;
            break;
        }
    }

    if (!countryExists)
    {
        (*countrySet) = realloc(*countrySet, (*count + 1) * sizeof(char *));
        (*countrySet)[*count] = strdup(node->country);
        (*count)++;
    }

    // Traverse right subtree
    countCountriesHelper(node->right, countrySet, count);
}

int countCountries(TS *ts)
{
    char **countrySet = NULL;
    int count = 0;

    countCountriesHelper(ts, &countrySet, &count);

    // Clean up the country set
    for (int i = 0; i < count; i++)
    {
        free(countrySet[i]);
    }
    free(countrySet);

    return count - 1; // unknown can't be considered
}

void playerWithFourStreaksHelper(TS *ts, bool retired, bool exists)
{
    if (ts == NULL)
    {
        return;
    }

    if (ts->left)
    {
        playerWithFourStreaksHelper(ts->left, retired, exists);
    }

    if (ts->mostTrophiesCount == 4 && ts->retired == retired)
    {
        printf("Player: %s\n", ts->name);
        printf("Streak Year: %d\n", ts->mostTrophiesYear);
        exists = true;
    }

    if (ts->right)
    {
        playerWithFourStreaksHelper(ts->right, retired, exists);
    }
}

bool playerWithFourStreaks(TS *ts, bool retired)
{
    bool exists = false;
    if (retired)
    {
        printf("Retired players with 4 Grand Slams:\n");
        playerWithFourStreaksHelper(ts, retired, exists);
    }
    else
    {
        printf("Active players with 4 Grand Slams:\n");
        playerWithFourStreaksHelper(ts, retired, exists);
    }
}

void displayMenu()
{
    printf("Menu:\n");
    printf("1. Print Active Players & Total Number of Active Players\n");
    printf("2. Print Retired Players & Total Number of Retired Players\n");
    printf("3. Print Retired Players by Accumulated Points\n");
    printf("4. Print Active Players by Grand Slam Ranking\n");
    printf("5. Print Retired Players with Four Grand Slams in a Single Year\n");
    printf("6. Print Active Players with Four Grand Slams in a Single Year\n");
    printf("7. Remove Active Players by Country\n");
    printf("8. Quit\n");
    printf("Enter your choice (1-8): ");
}

void handleMenuChoice(TS *ts, int choice)
{
    switch (choice)
    {
    case 1:
        printPlayersDescending(ts, false);
        break;
    case 2:
        printPlayersDescending(ts, true);
        break;
    case 3:
        printRetiredPlayersByAccumulatedPoints(ts);
        break;
    case 4:
        printPlayersByGrandSlamRanking(ts);
        break;
    case 5:
        playerWithFourStreaks(ts, true);
        break;
    case 6:
        playerWithFourStreaks(ts, false);
        break;
    case 7:
    {
        printf("Enter the country to remove active players (format should be = [country]): ");
        char country[50];
        scanf("%s", country);
        removeActivePlayersByCountry(ts, country);
        break;
    }
    case 8:
        printf("Quitting...\n");
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
}

int main()
{
    TS *ts = initialize();
    addDataOne(&ts); // initialize the tree with the data from jog_atuais.txt
    addDataTwo(&ts); // initialize the tree with the data from era_aberta_grand_slams.txt

    int choice;
    do
    {
        displayMenu();
        scanf("%d", &choice);
        printf("\n");

        handleMenuChoice(ts, choice);

        printf("\n");
    } while (choice != 8);

    // left - B
    freePlayers(ts);
    return 0;
}
