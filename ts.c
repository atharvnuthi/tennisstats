#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bTree.h"
#include "addData.h"

void printPlayersWithTitleDescending(TS *ts, bool retired)
{
    // Collect all players according to the retired status
    P *players[countAllPlayers(ts)];
    int index = 0;
    void storePlayersDescendingHelper(TS * ts, P * *players, int *index, bool retired)
    {
        if (!ts)
            return;
        for (int i = 0; i < ts->nKeys; i++)
        {
            if ((retired && ts->aKeys[i]->retired) || (!retired && !ts->aKeys[i]->retired)) // player is retired or active
            {
                if (ts->aKeys[i]->titles > 0) // has grand slams
                {
                    players[*index] = ts->aKeys[i];
                    (*index)++;
                }
            }
        }
        for (int j = 0; j < ts->nKeys + 1; j++)
        {
            storePlayersDescendingHelper(ts->childs[j], players, index, retired);
        }
    }
    storePlayersDescendingHelper(ts, players, &index, retired);

    // Sort players by titles in descending order using selection sort
    void sortPlayersByTitles(P * *players, int length)
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
                P *temp = players[i];
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
    void collectRetiredPlayers(TS * ts, P * *players, int *index)
    {
        if (!ts)
            return;
        for (int i = 0; i < ts->nKeys; i++)
        {
            if (ts->aKeys[i]->retired)
            {
                players[*index] = ts->aKeys[i];
                (*index)++;
            }
        }
        for (int j = 0; j < ts->nKeys + 1; j++)
        {
            collectRetiredPlayers(ts->childs[j], players, index);
        }
    }
    P *players[countAllPlayers(ts)];
    int index = 0; // index of players array, used to keep track of number of players
    collectRetiredPlayers(ts, players, &index);

    // Sort players by accumulated points in descending order using insertion sort
    void insertionSort(P * *arr, int size)
    {
        int i, j;
        P *key;

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
    void collectActivePlayersWithTitles(TS * ts, P * *players, int *index)
    {
        if (!ts)
            return;
        for (int i = 0; i < ts->nKeys; i++)
        {
            if (ts->aKeys[i]->retired == false && ts->aKeys[i]->titles > 0) // not retired & has grand slams
            {
                players[*index] = ts->aKeys[i];
                (*index)++;
            }
        }
        for (int j = 0; j < ts->nKeys + 1; j++)
        {
            collectActivePlayersWithTitles(ts->childs[j], players, index);
        }
    }
    P *players[countAllPlayers(ts)];
    int index = 0; // index of players array, used to keep track of number of players
    collectActivePlayersWithTitles(ts, players, &index);

    // Sort players by titles in descending order
    void bubbleSortPlayers(P * players[], int numPlayers)
    {
        int i, j;
        for (i = 0; i < numPlayers - 1; i++)
        {
            for (j = 0; j < numPlayers - i - 1; j++)
            {
                if (players[j]->titles < players[j + 1]->titles)
                {
                    // Swap players
                    P *temp = players[j];
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

void playerWithFourStreaks(TS *ts, bool retired)
{
    void playerWithFourStreaksHelper(TS * ts, bool retired)
    {
        if (ts == NULL)
        {
            return;
        }

        for (int i = 0; i < ts->nKeys; i++)
        {
            if (ts->aKeys[i]->mostTrophiesCount == 4 && ts->aKeys[i]->retired == retired)
            {
                printf("Player: %s\n", ts->aKeys[i]->name);
                printf("Streak Year: %d\n", ts->aKeys[i]->mostTrophiesYear);
            }
        }
        for (int j = 0; j < ts->nKeys + 1; j++)
        {
            playerWithFourStreaksHelper(ts->childs[j], retired);
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

void printActiveCompatriotsWithOneTitleAndSameAge(TS *ts)
{
    // Collect all active players with one title
    void collectActivePlayersWithOneTitle(TS * ts, P * *players, int *index)
    {
        if (!ts)
            return;

        for (int i = 0; i < ts->nKeys; i++)
        {
            if (!ts->aKeys[i]->retired && ts->aKeys[i]->titles == 1)
            {
                players[*index] = ts->aKeys[i];
                (*index)++;
            }
        }

        for (int j = 0; j < ts->nKeys + 1; j++)
        {
            collectActivePlayersWithOneTitle(ts->childs[j], players, index);
        }
    }

    // Filter players to find compatriots with one title from the same country and same age
    void findCompatriotsWithOneTitleAndSameAge(P * *players, int count)
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = i + 1; j < count; j++)
            {
                if (strcmp(players[i]->country, players[j]->country) == 0 && players[i]->age == players[j]->age)
                {
                    printPlayer(players[i]);
                    printPlayer(players[j]);
                }
            }
        }
    }

    // Collect all active players with one title
    P *players[countAllPlayers(ts)];
    int index = 0;
    collectActivePlayersWithOneTitle(ts, players, &index);

    printf("Active Compatriots with One Title and Same Age:\n");
    // Filter and print compatriots with one title from the same country
    findCompatriotsWithOneTitleAndSameAge(players, index);
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
    printf("8. Print Active Compatriots with One Title and Same Age\n");                // B
    printf("9. Quit\n");
    printf("Enter your choice (1-9): ");
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
            printf("Yet to be implemented.\n");
            // printf("Enter the country to remove active players (format should be = [country]): ");
            //  char country[50];
            //  scanf("%s", country);
            //  ts = removeActivePlayersByCountry(ts, country);
            break;
        }
        case 8:
        {
            printActiveCompatriotsWithOneTitleAndSameAge(ts);
            break;
        }
        case 9:
            printf("Quitting...\n");
            break;
        default:
            printf("Invalid choice. Leaving...\n");
            choice = 9;
            break;
        }

        printf("\n");
    } while (choice != 9);
}

int main()
{
    // Initialize
    int t = 2; // minimum degree of the tree
    TS *ts = initialize();
    addDataOne(&ts, t); // initialize the tree with the data from jog_atuais.txt
    addDataTwo(&ts, t); // initialize the tree with the data from era_aberta_grand_slams.txt

    // Main
    printf("---- Welcome to Tennis Stats! ----\n");
    handleMenu(ts);
    printf("Thanks for using Tennis Stats!\n");

    // Extra
    // printf("%d", countAllPlayers(ts));

    // Terminate
    // freeAllPlayers(ts);
    return 0;
}