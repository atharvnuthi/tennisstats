#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arv_bin.h"
#include "add.h"

void printPlayersWithTitleDescending(TS *ts, bool retired)
{
    // Collect all players according to the retired status
    TS *players[countAllPlayers(ts)];
    int index = 0;
    void storePlayersDescendingHelper(TS * ts, TS * *players, int *index, bool retired)
    {
        if (ts != NULL)
        {
            storePlayersDescendingHelper(ts->left, players, index, retired);

            if ((retired && ts->retired) || (!retired && !ts->retired)) // player is retired or active
            {
                if (ts->titles > 0) // has grand slams
                {
                    players[*index] = ts;
                    (*index)++;
                }
            }

            storePlayersDescendingHelper(ts->right, players, index, retired);
        }
    }
    storePlayersDescendingHelper(ts, players, &index, retired);

    // Sort players by titles in descending order using selection sort
    void sortPlayersByTitles(TS * *players, int length)
    {
        for (int i = 0; i < length - 1; i++)
        {
            int maxIndex = i;
            for (int j = i + 1; j < length; j++)
            {
                if (players[j]->titles > players[maxIndex]->titles)
                {
                    maxIndex = j;
                }
            }
            if (maxIndex != i)
            {
                TS *temp = players[i];
                players[i] = players[maxIndex];
                players[maxIndex] = temp;
            }
        }
    }
    sortPlayersByTitles(players, index);

    // Print players in decreasing order
    for (int i = 0; i < index; i++)
    {
        printPlayer(players[i]);
    }

    printf("Total Players: %d\n", index);
}

void printRetiredPlayersByAccumulatedPoints(TS *ts)
{
    // Collect all retired players
    void collectRetiredPlayers(TS * ts, TS * *players, int *index)
    {
        if (ts != NULL)
        {
            collectRetiredPlayers(ts->left, players, index);

            if (ts->retired)
            {
                players[*index] = ts;
                (*index)++;
            }

            collectRetiredPlayers(ts->right, players, index);
        }
    }
    TS *players[countAllPlayers(ts)];
    int index = 0; // index of players array, used to keep track of number of players
    collectRetiredPlayers(ts, players, &index);

    // Sort players by accumulated points in descending order using insertion sort
    void insertionSort(TS * *arr, int size)
    {
        int i, j;
        TS *key;

        for (i = 1; i < size; i++)
        {
            key = arr[i];
            j = i - 1;

            // Compare accumulated points in descending order
            while (j >= 0 && arr[j]->accpoints < key->accpoints)
            {
                arr[j + 1] = arr[j];
                j = j - 1;
            }

            arr[j + 1] = key;
        }
    }
    insertionSort(players, index);

    // Print players in descending order of points
    for (int i = 0; i < index; i++)
    {
        printf("Player Name: %s\n", players[i]->name);
        printf("Accumulated Points: %d\n", players[i]->accpoints);
        printf("------------------------\n");
    }
}

void printActivePlayersByGrandSlamRanking(TS *ts)
{
    // Collect all active players with grand slams
    void collectActivePlayersWithTitles(TS * ts, TS * *players, int *index)
    {
        if (ts != NULL)
        {
            collectActivePlayersWithTitles(ts->left, players, index);

            if (ts->retired == false && ts->titles > 0) // not retired & has grand slams
            {
                players[*index] = ts;
                (*index)++;
            }

            collectActivePlayersWithTitles(ts->right, players, index);
        }
    }
    TS *players[countAllPlayers(ts)];
    int index = 0; // index of players array, used to keep track of number of players
    collectActivePlayersWithTitles(ts, players, &index);

    // Sort players by titles in descending order
    void bubbleSortPlayers(TS * players[], int numPlayers)
    {
        int i, j;
        for (i = 0; i < numPlayers - 1; i++)
        {
            for (j = 0; j < numPlayers - i - 1; j++)
            {
                if (players[j]->titles < players[j + 1]->titles)
                {
                    // Swap players
                    TS *temp = players[j];
                    players[j] = players[j + 1];
                    players[j + 1] = temp;
                }
            }
        }
    }
    bubbleSortPlayers(players, index);

    // Print players in descending order of grand slam ranking
    for (int i = 0; i < index; i++)
    {
        printf("Player Name: %s\n", players[i]->name);
        printf("Grand Slam Ranking: %d\n", players[i]->titles);
        printf("------------------------\n");
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

bool playerWithFourStreaks(TS *ts, bool retired)
{
    void playerWithFourStreaksHelper(TS * ts, bool retired)
    {
        if (ts == NULL)
        {
            return;
        }

        if (ts->left)
        {
            playerWithFourStreaksHelper(ts->left, retired);
        }

        if (ts->mostTrophiesCount == 4 && ts->retired == retired)
        {
            printf("Player: %s\n", ts->name);
            printf("Streak Year: %d\n", ts->mostTrophiesYear);
        }

        if (ts->right)
        {
            playerWithFourStreaksHelper(ts->right, retired);
        }
    }

    if (retired) // retired players
    {
        printf("Retired players with 4 Grand Slams:\n");
        playerWithFourStreaksHelper(ts, retired);
    }
    else // active players
    {
        printf("Active players with 4 Grand Slams:\n");
        playerWithFourStreaksHelper(ts, retired);
    }
}

void displayMenu()
{
    printf("1. Print Retired Players & Total Number of Retired Players with Titles\n"); // A
    printf("2. Print Active Players & Total Number of Active Players with Titles\n");   // A
    printf("3. Print Retired Players with their Accumulated Points\n");                 // C
    printf("4. Print Active Players by their Grand Slam Ranking\n");                    // D
    printf("5. Print Retired Players with Four Grand Slams in a Single Year\n");        // E
    printf("6. Print Active Players with Four Grand Slams in a Single Year\n");         // F
    printf("7. Remove Active Players by their Country\n");                              // G
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
            printPlayersWithTitleDescending(ts, true);
            break;
        case 2:
            printPlayersWithTitleDescending(ts, false);
            break;
        case 3:
            printRetiredPlayersByAccumulatedPoints(ts);
            break;
        case 4:
            printActivePlayersByGrandSlamRanking(ts);
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
            printf("Invalid choice. Leaving...\n");
            choice = 8;
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
    printf("---- Welcome to Tennis Stats! ----\n");
    handleMenu(ts);
    printf("Thanks for using Tennis Stats!\n");

    // Extra
    // printf("%d", countAllPlayers(ts));

    // Terminate
    freePlayers(ts);
    return 0;
}