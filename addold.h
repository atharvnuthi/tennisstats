

void calculateAccumulatedPoints(TS *ts) // calculate the accumulated points of a player after retirement
{
    if (ts == NULL)
    {
        return;
    }

    calculateAccumulatedPoints(ts->left);

    if (ts->retired)
    {
        int points = ts->titles * 2000 + ts->subtitles * 1200;
        ts->points = -1;
        ts->accpoints = points;
    }

    calculateAccumulatedPoints(ts->right);
}

// Calculate accumulated points for players with retired = true
calculateAccumulatedPoints(*ts);
--end of dataTwo