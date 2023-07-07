// strlen - length of a string
// strtok - split a string into tokens
// atoi - convert a string to an integer
// fgets - read a line from a file
// fopen - open a file

#define MAX_LINE_LENGTH 100

void addDataOne(TS **ts, int t)
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
        int len = strlen(fileLine);
        if (fileLine[len - 1] == '\n') // end of line
        {
            fileLine[len - 1] = '\0'; // null of line
        }

        char *currentRank = strtok(fileLine, "\t");
        char *country = strtok(NULL, "\t");
        char *name = strtok(NULL, "\t");
        char *points = strtok(NULL, "\t");
        char *age = strtok(NULL, "\t");

        *ts = insertPlayer(*ts, createPlayer(name, country, false, 0, 0, atoi(age), atoi(currentRank), atoi(points), 0, 0), t);
    }
    fclose(filePtr);
}

void calculateAccumulatedPoints(TS *ts) // calculate the accumulated points of a player after retirement -logic in b-tree
{
    if (!ts)
    {
        return;
    }
    for (int i = 0; i < ts->nKeys; i++)
    {
        calculateAccumulatedPoints(ts->childs[i]);
        if (ts->aKeys[i]->retired)
        {
            int points = ts->aKeys[i]->titles * 2000 + ts->aKeys[i]->subtitles * 1200;
            ts->aKeys[i]->points = -1;
            ts->aKeys[i]->accpoints = points;
        }
    }
}

void addDataTwo(TS **ts, int t)
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
        int len = strlen(fileLine);
        if (fileLine[len - 1] == '\n')
        {
            fileLine[len - 1] = '\0';
        }

        char *year = strtok(fileLine, "\t");
        char *tournament = strtok(NULL, "\t");
        char *winner = strtok(NULL, "\t");   // winner's name
        char *runnerUp = strtok(NULL, "\t"); // runner-up's name

        // Check if the winner player already exists in the binary tree
        P *existingWinner = searchByName(*ts, winner);
        if (existingWinner == NULL)
        {
            // Add a new player with the winner's data
            P *newWinner = createPlayer(winner, "[Unknown]", true, 1, 0, -1, -1, 0, atoi(year), 1);
            *ts = insertPlayer(*ts, newWinner, t);
            existingWinner = newWinner;
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
        P *existingRunnerUp = searchByName(*ts, runnerUp);
        if (existingRunnerUp == NULL)
        {
            // Add a new player with the runner-up's data
            P *newRunnerUp = createPlayer(runnerUp, "[Unknown]", true, 0, 1, -1, -1, 0, 0, 0);
            *ts = insertPlayer(*ts, newRunnerUp, t);
            existingRunnerUp = newRunnerUp;
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

    // Calculate accumulated points for players with retired = true
    calculateAccumulatedPoints(*ts);
}