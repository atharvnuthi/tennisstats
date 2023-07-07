#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bTree.h"
#include "addData.h"

int main()
{
    int t = 2; // minimum degree of the tree
    TS *ts = initialize();
    addDataOne(&ts, t); // initialize the tree with the data from jog_atuais.txt
    addDataTwo(&ts, t); // initialize the tree with the data from era_aberta_grand_slams.txt
    calculateAccumulatedPoints(ts);

    printPlayer(searchByName(ts, "Roger Federer"));
    return 0;
}