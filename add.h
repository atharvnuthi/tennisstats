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
