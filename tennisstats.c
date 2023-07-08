#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arv_bin.h"
#include "add.h"

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

void removeActivePlayersByCountry(TS *ts, char)