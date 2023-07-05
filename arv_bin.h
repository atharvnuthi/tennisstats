// strcmp - compare two strings lexicographically , 0/-1/1
// strdup - copy a string to a new memory location and return a pointer to it

#define MAX_LINE_LENGTH 100

typedef struct tennisstats
{
    // general information
    char *name;
    char *country;
    bool retired;
    int age;

    // ranking and points
    int currentRank;
    int points;
    int accpoints; // points accumulated after retirement, active player will have the same value as points

    // titles and grand slams
    int titles;    // winner of the tournament
    int subtitles; // runner-up of the tournament
    int numOfAO;   // australian open titles
    int numOfW;    // wimbledon titles
    int numOfFO;   // french open titles
    int numOfUSO;  // us open titles

    // streak is the number of grand slams won in a year
    int mostTrophiesCount; // number of streak
    int mostTrophiesYear;  // year of streak

    // helper variables for streak count
    int prevStreakCount;
    int streakYear;
    int streakCount;

    struct tennisstats *left, *right; // left and right child
} TS;

TS *initialize()
{
    return NULL;
}

TS *createPlayer(char *name, char *country, bool retired, int titles, int subtitles, int age, int currentRank, int points, int streakYear, int streakCount)
{
    TS *newPlayer = (TS *)malloc(sizeof(TS));
    newPlayer->name = strdup(name);
    newPlayer->country = strdup(country);
    newPlayer->retired = retired;
    newPlayer->titles = titles;
    newPlayer->subtitles = subtitles;
    newPlayer->age = age;
    newPlayer->currentRank = currentRank;
    newPlayer->points = points;
    newPlayer->accpoints = points;

    newPlayer->numOfAO = 0;
    newPlayer->numOfW = 0;
    newPlayer->numOfFO = 0;
    newPlayer->numOfUSO = 0;
    newPlayer->mostTrophiesCount = 0;
    newPlayer->mostTrophiesYear = 0;
    newPlayer->prevStreakCount = 0;

    newPlayer->streakYear = streakYear;
    newPlayer->streakCount = streakCount;

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
    if (strcmp(player->name, ts->name) < 0) // player name is lexicographically smaller than ts->name
    {
        ts->left = insertPlayer(ts->left, player);
    }
    else
    {
        ts->right = insertPlayer(ts->right, player); // player name is lexicographically larger than ts->name
    }
    return ts;
}

TS *searchByName(TS *ts, char *name)
{
    if (ts == NULL || strcmp(name, ts->name) == 0) // player name is lexicographically equal to ts->name
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
    else // player name is lexicographically equal to ts->name
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
        else // both left and right are not NULL
        {
            TS *temp = ts->right;
            while (temp->left != NULL)
            {
                temp = temp->left; // find the leftmost node in the right subtree
            }
            free(ts->name);
            free(ts->country);
            ts->name = strdup(temp->name);
            ts->country = strdup(temp->country);
            ts->retired = temp->retired;
            ts->titles = temp->titles;
            ts->subtitles = temp->subtitles;
            ts->age = temp->age;
            ts->currentRank = temp->currentRank;
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
            ts->right = removePlayer(ts->right, temp->name); // remove the leftmost node which is a duplicate in the right subtree
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
    printf("Rank: %d\n", player->currentRank);
    printf("Points: %d\n", player->points);
    printf("Australian Open Titles: %d\n", player->numOfAO);
    printf("Wimbledon Titles: %d\n", player->numOfW);
    printf("French Open Titles: %d\n", player->numOfFO);
    printf("US Open Titles: %d\n", player->numOfUSO);
    printf("------------------------\n");
}

void printAllPlayers(TS *ts)
{
    if (!ts)
        return;
    printPlayer(ts);
    printAllPlayers(ts->left);
    printAllPlayers(ts->right);
}

int countAllPlayers(TS *ts)
{
    if (!ts)
    {
        return 0;
    }
    return 1 + countAllPlayers(ts->left) + countAllPlayers(ts->right);
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