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
        if (ts->left == NULL && ts->right == NULL)
        {
            free(ts->name);
            free(ts->country);
            free(ts);
            return NULL;
        }
        else if (ts->left == NULL)
        {
            TS *temp = ts->right;
            free(ts->name);
            free(ts->country);
            free(ts);
            return temp;
        }
        else if (ts->right == NULL)
        {
            TS *temp = ts->left;
            free(ts->name);
            free(ts->country);
            free(ts);
            return temp;
        }
        else
        {
            TS *temp = ts->right;
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            free(ts->name);
            free(ts->country);
            ts->name = strdup(temp->name);
            ts->country = strdup(temp->country);
            ts->retired = temp->retired;
            ts->titles = temp->titles;
            ts->subtitles = temp->subtitles;
            ts->age = temp->age;
            ts->rank = temp->rank;
            ts->points = temp->points;
            ts->accpoints = temp->accpoints;
            ts->numOfAO = temp->numOfAO;
            ts->numOfW = temp->numOfW;
            ts->numOfFO = temp->numOfFO;
            ts->numOfUSO = temp->numOfUSO;
            ts->prevStreakCount = temp->prevStreakCount;
            ts->streakYear = temp->streakYear;
            ts->streakCount = temp->streakCount;
            ts->mostTrophiesCount = temp->mostTrophiesCount;
            ts->mostTrophiesYear = temp->mostTrophiesYear;
            ts->right = removePlayer(ts->right, temp->name);
        }
    }
    return ts;
}

void printPlayer(TS *player)
{
    printf("------------------------\n");
    printf("Name: %s\n", player->name);
    printf("Age: %d\n", player->age);
    printf("Country: %s\n", player->country);
    printf("Retired: %s\n", player->retired ? "Yes" : "No");
    printf("No. of Titles: %d\n", player->titles);
    printf("No. of Subtitles: %d\n", player->subtitles);
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