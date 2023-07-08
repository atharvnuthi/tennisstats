typedef struct P
{
    // General information
    char *name;
    char *country;
    bool retired;
    int age;

    // Ranking and points
    int currentRank;
    int points;
    int accpoints; // Points accumulated after retirement, active player will have the same value as points

    // Titles and Grand Slams
    int titles;    // Winner of the tournament
    int subtitles; // Runner-up of the tournament
    int numOfAO;   // Australian Open titles
    int numOfW;    // Wimbledon titles
    int numOfFO;   // French Open titles
    int numOfUSO;  // US Open titles

    // Streak is the number of Grand Slams won in a year
    int mostTrophiesCount; // Number of streak
    int mostTrophiesYear;  // Year of streak

    // Helper variables for streak count
    int prevStreakCount;
    int streakYear;
    int streakCount;
} P;

P *createPlayer(char *name, char *country, bool retired, int titles, int subtitles, int age, int currentRank, int points, int streakYear, int streakCount)
{
    P *newPlayer = (P *)malloc(sizeof(P));
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

    return newPlayer;
}

typedef struct TS
{
    int nKeys;          // Number of keys filled
    bool leaf;          // Is leaf or not
    P **aKeys;          // Array of players
    struct TS **childs; // Children pointers (array of pointers)
} TS;

int countAllPlayers(TS *ts)
{
    if (ts)
    {
        int i, count = 0;
        for (i = 0; i < ts->nKeys; i++)
        {
            count += countAllPlayers(ts->childs[i]);
            count++;
        }
        count += countAllPlayers(ts->childs[i]);
        return count;
    }
    return 0;
}

TS *initialize()
{
    return NULL;
}

P *searchByName(TS *ts, char *name)
{
    if (!ts)
    {
        return NULL;
    }
    int i = 0;
    while (i < ts->nKeys && strcmp(name, ts->aKeys[i]->name) > 0)
    {
        i++;
    }
    if (i < ts->nKeys && strcmp(name, ts->aKeys[i]->name) == 0)
    {
        return ts->aKeys[i];
    }
    if (ts->leaf)
    {
        return NULL;
    }
    return searchByName(ts->childs[i], name);
}

void printPlayer(P *player)
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
    if (ts)
    {
        int i;
        for (i = 0; i < ts->nKeys; i++)
        {
            printAllPlayers(ts->childs[i]);
            printPlayer(ts->aKeys[i]);
        }
        printAllPlayers(ts->childs[i]);
    }
}

TS *create(int t)
{
    TS *ts = (TS *)malloc(sizeof(TS));
    ts->nKeys = 0;
    ts->leaf = true;
    ts->aKeys = (P **)malloc(sizeof(P *) * ((2 * t) - 1));
    ts->childs = (TS **)malloc(sizeof(TS *) * (2 * t));
    for (int i = 0; i < (2 * t); i++)
    {
        ts->childs[i] = NULL;
    }
    return ts;
}

TS *divide(TS *x, int idx, TS *y, int t)
{
    TS *z = create(t);
    z->nKeys = t - 1;
    z->leaf = y->leaf;
    int j;
    for (j = 0; j < t - 1; j++)
    {
        z->aKeys[j] = y->aKeys[j + t];
    }
    if (!y->leaf)
    {
        for (j = 0; j < t; j++)
        {
            z->childs[j] = y->childs[j + t];
            y->childs[j + t] = NULL;
        }
    }
    y->nKeys = t - 1;
    for (j = x->nKeys; j >= idx; j--)
    {
        x->childs[j + 1] = x->childs[j];
    }
    x->childs[idx] = z;
    for (j = x->nKeys; j >= idx; j--)
    {
        x->aKeys[j] = x->aKeys[j - 1];
    }
    x->aKeys[idx - 1] = y->aKeys[t - 1];
    x->nKeys++;
    return x;
}

TS *insertPlayerHelper(TS *ts, P *p, int t)
{
    int i = ts->nKeys - 1;
    if (ts->leaf)
    {
        while (i >= 0 && strcmp(p->name, ts->aKeys[i]->name) < 0)
        {
            ts->aKeys[i + 1] = ts->aKeys[i];
            i--;
        }
        ts->aKeys[i + 1] = p;
        ts->nKeys++;
        return ts;
    }
    while (i >= 0 && strcmp(p->name, ts->aKeys[i]->name) < 0)
    {
        i--;
    }
    i++;
    if (ts->childs[i]->nKeys == ((2 * t) - 1))
    {
        ts = divide(ts, (i + 1), ts->childs[i], t);
        if (strcmp(p->name, ts->aKeys[i]->name) > 0)
        {
            i++;
        }
    }
    ts->childs[i] = insertPlayerHelper(ts->childs[i], p, t);
    return ts;
}

TS *insertPlayer(TS *ts, P *p, int t)
{
    if (searchByName(ts, p->name))
    {
        return ts;
    }
    if (!ts)
    {
        ts = create(t);
        ts->aKeys[0] = p;
        ts->nKeys = 1;
        return ts;
    }
    if (ts->nKeys == (2 * t) - 1)
    {
        TS *new = create(t);
        new->nKeys = 0;
        new->leaf = false;
        new->childs[0] = ts;
        new = divide(new, 1, ts, t);
        new = insertPlayerHelper(new, p, t);
        return new;
    }
    ts = insertPlayerHelper(ts, p, t);
    return ts;
}

void freePlayer(P *p)
{
    if (p)
    {
        free(p->name);
        free(p->country);
        free(p);
    }
}

void freeAllPlayers(TS *ts)
{
    if (ts)
    {
        if (!ts->leaf)
        {
            for (int i = 0; i <= ts->nKeys; i++)
            {
                freeAllPlayers(ts->childs[i]);
            }
        }
        for (int i = 0; i < ts->nKeys; i++)
        {
            freePlayer(ts->aKeys[i]);
        }
        free(ts->aKeys);
        free(ts->childs);
        free(ts);
    }
}
