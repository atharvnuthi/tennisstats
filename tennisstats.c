#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arv.h"
#include "add.h"

int comparePlayersHelper(const void *a, const void *b)
{
    TS *playerA = *(TS **)a;
    TS *playerB = *(TS **)b;

    // Compare by titles in descending order
    return playerB->titles - playerA->titles;
}

void printPlayersWithTitleDescending(TS *ts, bool retired)
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
    qsort(players, index, sizeof(TS *), comparePlayersHelper);

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

    int comparePlayersHelper(const void *a, const void *b)
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
    qsort(players, index, sizeof(TS *), comparePlayersHelper);

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

    int comparePlayersHelper(const void *a, const void *b)
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
    qsort(players, index, sizeof(TS *), comparePlayersHelper);

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
    printf("1. Print Active Players & Total Number of Active Players with Titles\n");
    printf("2. Print Retired Players & Total Number of Retired Players with Titles\n");
    printf("3. Print Retired Players with their Accumulated Points\n");
    printf("4. Print Active Players by their Grand Slam Ranking\n");
    printf("5. Print Retired Players with Four Grand Slams in a Single Year\n");
    printf("6. Print Active Players with Four Grand Slams in a Single Year\n");
    printf("7. Remove Active Players by their Country\n");
    printf("8. Quit\n");
    printf("Enter your choice (1-8): ");
}

void handleMenu(TS *ts)
{
    int choice;
    do
    {
        displayMenu();
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
        case 1:
            printPlayersWithTitleDescending(ts, false);
            break;
        case 2:
            printPlayersWithTitleDescending(ts, true);
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

        printf("\n");
    } while (choice != 8);
}

int main()
{
    // Initialize
    TS *ts = initialize();
    addDataOne(&ts); // initialize the tree with the data from jog_atuais.txt
    addDataTwo(&ts); // initialize the tree with the data from era_aberta_grand_slams.txt

    // Main
    handleMenu(ts);

    // left - B

    // Terminate
    freePlayers(ts);
    return 0;
}